/*
 * A237053  Smallest number k such that some subset of n+1..n+k can be summed
 *          and added to n to produce a prime.
 * https://oeis.org/A237053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 9809, u32 n <= 9809, u64 n <= 9809,
 *            u128 n <= 9809, big any n
 * Helpers:   isPrimeGeneric
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
#include "../utils/isPrimeGeneric.h"

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
