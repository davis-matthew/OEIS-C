/*
 * A088572  Numbers n such that (2n+1)^2 - 2 is prime.
 * https://oeis.org/A088572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 52, u32 n <= 6557, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 8863 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A088572: numbers n such that (2n+1)^2 – 2 is prime.
   The function uses a simple search with a cached list of results.
   For seq_t sizes up to 64 bits a fast deterministic Miller–Rabin test
   is employed; for larger seq_t types a generic big‑integer test is
   used. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
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

/* ---------- fast 64‑bit primality test ----------------------------- */
/* Deterministic Miller–Rabin for values < 3 474 749 660 383,
   which comfortably covers all testable numbers in this sequence. */
static bool is_prime_seq(seq_t n)
{
    if (sizeof(seq_t) <= 8) {
        uint64_t un = (uint64_t)n;
        if (un < 2) return false;

        /* small primes first */
        static const uint64_t small_primes[] =
            {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
        for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
            if (un % small_primes[i] == 0) return un == small_primes[i];
        }

        /* factor n-1 as d * 2^s */
        uint64_t d = un - 1;
        int s = 0;
        while ((d & 1ULL) == 0) { d >>= 1; ++s; }

        /* bases adequate for < 3·10^12 */
        static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL};
        for (size_t bi = 0; bi < sizeof(bases)/sizeof(bases[0]); ++bi) {
            uint64_t a = bases[bi];
            if (a % un == 0) continue;

            /* modular exponentiation: x = a^d mod un */
            uint64_t x = 1, pow = a % un;
            uint64_t exp = d;
            while (exp) {
                if (exp & 1ULL) x = (unsigned __int128)x * pow % un;
                pow = (unsigned __int128)pow * pow % un;
                exp >>= 1;
            }

            if (x == 1 || x == un - 1) continue;

            bool composite = true;
            for (int r = 1; r < s; ++r) {
                x = (unsigned __int128)x * x % un;
                if (x == un - 1) { composite = false; break; }
            }
            if (composite) return false;
        }
        return true;
    } else {
        /* arbitrary precision fallback */
        return is_prime_big(n);
    }
}

/* ---------- sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n == 0) return 0;                /* offset is 1; n<1 not used */

    while (cache_len < n) {
        idx_t start = (cache_len == 0) ? 1
                                        : (idx_t)cache[cache_len-1] + 1;

        for (idx_t cand = start; ; ++cand) {
            seq_t x = (seq_t)2 * (seq_t)cand + 1;
            seq_t p = x * x - 2;

            if (is_prime_seq(p)) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2 + 1;
                    grow_seq(&cache, &cache_cap, new_cap);
                }
                cache[cache_len++] = (seq_t)cand;
                break;
            }
        }
    }
    return cache[n-1];
}
