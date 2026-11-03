/*
 * A129926  Semiprimes s such that 3*s - 2 is a prime.
 * https://oeis.org/A129926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1383, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9872 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for semiprimes s such that 3*s-2 is a prime. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t next_candidate = 4;          /* start checking from 4 */

    /* Small buffers for the factor_u64 helper. */
    uint64_t pr[16];
    uint64_t ex[16];
    size_t cnt;

    /* Return a value we already know if enough terms have been
       found previously. */
    if ((idx_t)terms_len >= n) {
        return terms[n - 1];
    }

    /* Find the next terms until we have at least n terms stored. */
    while ((idx_t)terms_len < n) {
        uint64_t s = (uint64_t)next_candidate;

        /* Search for the next semiprime that satisfies the property. */
        while (1) {
            factor_u64(s, pr, ex, &cnt);

            /* Count the total multiplicity of prime factors. */
            size_t mult = 0;
            for (size_t i = 0; i < cnt; ++i) {
                mult += ex[i];
            }

            if (mult == 2) {                      /* s is a semiprime */
                uint64_t m = 3ULL * s - 2ULL;
                if (is_prime_big((seq_t)m)) {     /* 3*s-2 is prime */
                    append_candidate(&terms, &terms_len, &terms_cap,
                                     (seq_t)s);
                    break;                       /* found next term */
                }
            }
            ++s;                                  /* try next integer */
        }

        /* All future searches start after the one we just found. */
        next_candidate = s + 1;
    }

    return terms[n - 1];
}
