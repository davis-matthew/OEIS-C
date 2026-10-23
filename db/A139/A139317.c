/*
 * A139317  a(n) = the smallest value of the form n*k + 1, k = positive
 *          integer, that is coprime to all the previous terms of this
 *          sequence.
 * https://oeis.org/A139317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = the smallest value of the form n*k + 1 (k > 0)
 * that is coprime to all previous terms of the sequence.
 * The sequence is a permutation of the primes.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;   /* number of terms currently stored */
    static size_t seq_cap = 0;   /* capacity of the cache */

    if (n == 0) return 0;   /* undefined for n < 1, return 0 */

    /* grow the cache if we need more terms */
    while (seq_len < (size_t)n) {
        /* increase capacity */
        if (seq_cap == 0) {
            seq_cap = 64;
            seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        } else if (seq_len + 1 > seq_cap) {
            size_t new_cap = seq_cap * 2;
            if (new_cap < seq_len + 1) new_cap = seq_len + 1;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }

        /* term number we are computing (1‑based) */
        idx_t idx = seq_len + 1;

        /* search for the smallest k > 0 with the required property */
        seq_t k = 1;
        seq_t candidate;
        while (1) {
            candidate = ((seq_t)idx) * k + (seq_t)1;
            /* candidate must be prime (sequence is a permutation of primes) */
            if (!is_prime_big(candidate)) {
                ++k;
                continue;
            }
            /* check coprimality with all previous terms */
            bool coprime = true;
            for (size_t j = 0; j < seq_len; ++j) {
                if (candidate % seq[j] == 0) {
                    coprime = false;
                    break;
                }
            }
            if (coprime) {
                seq[seq_len++] = candidate;
                break;
            }
            ++k;
        }
    }

    return seq[n - 1];
}
