/*
 * A052290  Prime values of n!*2^n+n+1 = A000165(n)+n+1.
 * https://oeis.org/A052290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Prime values of n!*2^n + n + 1 (OEIS A052290)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   is_prime_u64, mem, mulmod, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  – a(1) is the first term (n = 0)             */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mulmod.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Deterministic primality test for 64‑bit values (deterministic bases). */
/* For larger seq_t types (arbitrary precision) we fall back to       */
/* the generic big‑integer test provided by the framework.            */
/* ------------------------------------------------------------------ */
static inline bool is_prime_seq(seq_t n)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    /* In C99 or later we can use inline */
#endif

    /* 64‑bit path ----------------------------------------------- */
    if (sizeof(seq_t) <= 8) {
        uint64_t v = (uint64_t)n;
        if (v < 2U)
            return false;
        if ((v & 1U) == 0)
            return v == 2U;

        /* Write v-1 = 2^r * d with d odd */
        uint64_t d = v - 1U;
        int r = 0;
        while ((d & 1U) == 0U) {
            d >>= 1U;
            ++r;
        }

        /* Deterministic bases for 64‑bit integers */
        static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,
                                         17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
        size_t i;
        for (i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
            uint64_t a = bases[i];
            if (a % v == 0U)  /* skip if a is a multiple of v */
                continue;

            uint64_t x = (uint64_t)powmod((seq_t)a, d, (seq_t)v);
            if (x == 1U || x == v - 1U)
                continue;

            int j;
            bool composite = true;
            for (j = 1; j < r; ++j) {
                x = mulmod(x, x, v);
                if (x == v - 1U) {
                    composite = false;
                    break;
                }
            }
            if (composite)
                return false;
        }
        return true;
    }

    /* arbitrary‑precision path ---------------------------------- */
    return is_prime_big(n);
}

/* ------------------------------------------------------------------ */
/* Main sequence function: a(n) = the n‑th prime of n!*2^n+n+1.       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Static state to allow calls in any order. */
    static seq_t *prime_vals = NULL;
    static size_t prime_cap = 0;   /* allocated capacity of prime_vals */
    static size_t prime_cnt = 0;   /* number of primes stored */

    /* current parameters for generating the next term */
    static idx_t  cur_n   = 0;     /* current n for which fac and pow hold */
    static seq_t  fac     = 1;     /* cur_n!  */
    static seq_t  pow2    = 1;     /* 2^cur_n */

    /* Generate primes until we have at least n of them. */
    while (prime_cnt < n) {
        seq_t val = fac * pow2 + (seq_t)cur_n + (seq_t)1;  /* n!*2^n + n + 1 */

        if (is_prime_seq(val)) {
            /* Ensure we have enough capacity. */
            if (prime_cnt == prime_cap) {
                grow_seq(&prime_vals, &prime_cap, prime_cap == 0 ? 4 : prime_cap * 2);
            }
            prime_vals[prime_cnt++] = val;
        }

        /* Advance to the next n for the next iteration. */
        ++cur_n;
        fac *= (seq_t)cur_n;     /* cur_n!  becomes (cur_n+1)!   */
        pow2 *= (seq_t)2;        /* 2^cur_n becomes 2^(cur_n+1) */
    }

    /* Indexes are 1‑based; array is 0‑based. */
    return prime_vals[n - 1];
}
