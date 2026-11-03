/*
 * A175517  Duplicate of A163678.
 * https://oeis.org/A175517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 none, i16 n <= 4, i32 n <= 34, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-03)
 * Helpers:   digit_counts, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A175517
 *  a(n) = the nth smaller prime p such that the next consecutive prime q
 *  satisfies q - p = 18 and the decimal representations of p and q are
 *  permutations (anagrams).
 *
 *  The function computes the sequence lazily and caches the results,
 *  so repeated calls with arbitrary n are handled efficiently.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"
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

seq_t a(idx_t n)
{
    /*  static cache of found terms  */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /*  static state for generating primes  */
    static seq_t last_prime = 0;

    /*  Ensure we have computed at least n terms  */
    if (cache_len < (size_t)n) {
        /*  Grow the cache if needed  */
        if (cache_cap < cache_len + 1) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
        }

        /*  Main generation loop  */
        while (cache_len < (size_t)n) {

            /*  find next prime after last_prime  */
            seq_t cand;
            if (last_prime == 0) {
                cand = 2;
            } else {
                cand = last_prime + 1;
                if (cand % 2 == 0) cand++;                 /* start with odd candidate */
                while (!is_prime_big(cand)) cand += 2;     /* skip evens */
            }

            /*  check gap 18  */
            if (last_prime > 0 && (cand - last_prime) == (seq_t)18) {
                /*  check anagram: compare digit counts  */
                unsigned char cnt1[10] = {0};
                unsigned char cnt2[10] = {0};
                digit_counts(last_prime, cnt1);
                digit_counts(cand, cnt2);
                bool anagram = true;
                for (int i = 0; i < 10; ++i) {
                    if (cnt1[i] != cnt2[i]) { anagram = false; break; }
                }
                if (anagram) {
                    /*  Store the smaller prime  */
                    if (cache_len >= cache_cap) {
                        grow_seq(&cache, &cache_cap, cache_len + 1);
                    }
                    cache[cache_len++] = last_prime;
                }
            }

            /*  move to next prime  */
            last_prime = cand;
        }
    }

    return cache[(size_t)n - 1];
}
