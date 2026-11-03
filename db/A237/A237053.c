/*
 * A237053  Smallest number k such that some subset of n+1..n+k can be summed
 *          and added to n to produce a prime.
 * https://oeis.org/A237053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 99999, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 113 terms through n = 9809 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A237053: Smallest k such that some subset of n+1..n+k can be summed
 * and added to n to produce a prime.  The function returns the value a(n)
 * for the given index n. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* compute the sequence value for a given index n */
seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;                     /* n cast to seq_t */

    /* n is prime → k = 0 */
    if (isPrimeGeneric(base))
        return 0;

    /* Bitset for attainable sums; initially only sum 0 is reachable */
    size_t word_count = 1;
    uint64_t *bitset  = (uint64_t *)calloc(word_count, sizeof(uint64_t));
    uint64_t *shifted = (uint64_t *)calloc(word_count, sizeof(uint64_t));
    if (!bitset || !shifted) {                /* memory failure – return 0 */
        free(bitset);
        free(shifted);
        return 0;
    }
    bitset[0] = 1ULL;                          /* bit 0 set: sum 0 reachable */

    seq_t sum_total = 0;                       /* total sum of numbers seen */
    idx_t  k        = 0;                       /* current block size (k) */

    for (;;) {
        /* extend block by one new integer: n + k + 1 */
        ++k;
        idx_t  val_idx = n + k;                /* integer value of the new number */
        seq_t val     = (seq_t)val_idx;

        /* update total sum and possibly enlarge the bitset */
        seq_t new_sum_total = sum_total + val;
        size_t new_word_count = (size_t)(new_sum_total / 64) + 1;
        if (new_word_count > word_count) {
            uint64_t *tmp = (uint64_t *)realloc(bitset,
                                                new_word_count * sizeof(uint64_t));
            if (!tmp) break;                  /* memory failure – exit loop */
            bitset = tmp;
            tmp = (uint64_t *)realloc(shifted,
                                      new_word_count * sizeof(uint64_t));
            if (!tmp) break;
            shifted = tmp;
            memset(bitset + word_count, 0,
                   (new_word_count - word_count) * sizeof(uint64_t));
            word_count = new_word_count;
        }

        /* shift the current bitset left by val bits and OR it in */
        memset(shifted, 0, word_count * sizeof(uint64_t));
        size_t shift_words = (size_t)((uint64_t)val >> 6);     /* value / 64 */
        uint64_t shift_bits = (uint64_t)val & 63ULL;          /* value % 64 */

        for (size_t i = 0; i < word_count; ++i) {
            uint64_t w = bitset[i];
            if (!w)
                continue;
            size_t dest = i + shift_words;
            if (dest >= word_count)
                continue;
            if (shift_bits == 0) {
                shifted[dest] |= w;
            } else {
                shifted[dest] |= w << shift_bits;
                if (dest + 1 < word_count)
                    shifted[dest + 1] |= w >> (64 - shift_bits);
            }
        }
        for (size_t i = 0; i < word_count; ++i)
            bitset[i] |= shifted[i];

        sum_total = new_sum_total;

        /* examine all reachable sums for primality after adding n */
        for (size_t wi = 0; wi < word_count; ++wi) {
            uint64_t w = bitset[wi];
            while (w) {
                uint64_t bit     = w & -w;                     /* least set bit */
                int      b_index = __builtin_ctzll(w);         /* bit position */
                seq_t   s = ((seq_t)wi << 6) + (seq_t)b_index; /* the sum value */
                seq_t   candidate = base + s;
                if (isPrimeGeneric(candidate)) {
                    free(bitset);
                    free(shifted);
                    return (seq_t)k;
                }
                w ^= bit;  /* clear processed bit */
            }
        }
    }

    /* Should never reach here – clean up and return 0 */
    free(bitset);
    free(shifted);
    return 0;
}
