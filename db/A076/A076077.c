/*
 * A076077  Group the natural numbers so that the n-th group contains n
 *          numbers, the k-th number in a group is a multiple of k and the sum
 *          of the group is prime: (2), (1, 4), (3, 8, 6), (5, 10, 12, 16),
 *          (7, 14, 9, 24, 25), ...
 * https://oeis.org/A076077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gcd, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A076077
   The n-th term is the n-th element of a sequence of groups:
   group n has n numbers T(n,1)…T(n,n).  T(n,k) is defined by
   - for 1 ≤ k ≤ n‑2: the smallest unused multiple of k;
   - T(n,n‑1) is the smallest unused multiple of n‑1 such that
     T(n,1)+…+T(n,n‑1) is coprime to n;
   - T(n,n) is the smallest unused multiple of n that makes the
     whole group sum prime.
   Each term is unique.  The function below recomputes from scratch
   for every call and is valid for any seq_t defined by the build.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"

/* ------------------------------------------------------------
   Bit‑set used for numbers that appear in the sequence.
   The numbers in the sequence stay reasonably small for the
   supported n; we cast them to uint64_t to index the bit‑set.
   ------------------------------------------------------------ */
static uint64_t *used_bits = NULL;   /* bit array, packed */
static size_t used_cap_words = 0;    /* number of words allocated */

/* Test if a value has already been used */
static inline bool used_check(seq_t x)
{
    size_t idx  = (size_t)(uint64_t)(x);       /* assume x fits in 64 bits */
    size_t word = idx >> 6;
    if (word >= used_cap_words) return false;
    return (used_bits[word] >> (idx & 63)) & 1ULL;
}

/* Mark a value as used */
static inline void used_set(seq_t x)
{
    size_t idx  = (size_t)(uint64_t)(x);   /* assume x fits in 64 bits */
    size_t word = idx >> 6;
    if (word >= used_cap_words) {
        size_t new_cap = word + 1;
        uint64_t *new_arr = (uint64_t *)realloc(used_bits, new_cap * sizeof(uint64_t));
        if (!new_arr) abort();             /* memory exhausted */
        /* initialize new words to 0 */
        for (size_t i = used_cap_words; i < new_cap; ++i) new_arr[i] = 0;
        used_bits = new_arr;
        used_cap_words = new_cap;
    }
    used_bits[word] |= (1ULL << (idx & 63));
}

/* Return the smallest unused multiple of m */
static inline seq_t next_unused_multiple(idx_t m)
{
    seq_t cand = (seq_t)m;
    while (used_check(cand)) cand += (seq_t)m;
    return cand;
}

/* ------------------------------------------------------------
   Compute the n-th term (1‑based index).  The function is
   self‑contained, thread‑safe, and does not rely on prior calls.
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for the offset specified */

    /* Reset the used set for each independent call. */
    if (used_bits) {
        free(used_bits);
        used_bits = NULL;
        used_cap_words = 0;
    }

    idx_t term_idx = 0;          /* number of produced terms so far */
    idx_t group    = 1;          /* current group size */

    while (term_idx < n) {
        seq_t sum = 0;           /* partial sum of the current group */

        /* 1 ≤ k ≤ group‑2: smallest unused multiple of k */
        for (idx_t k = 1; k + 1 < group; ++k) {
            seq_t val = next_unused_multiple(k);
            ++term_idx;
            if (term_idx == n) return val;
            used_set(val);
            sum += val;
        }

        /* k = group‑1: choose smallest unused multiple of k
           such that sum + val is coprime with group. */
        if (group > 1) {
            idx_t k = group - 1;
            seq_t val;
            for (seq_t cand = (seq_t)k; ; cand += (seq_t)k) {
                if (!used_check(cand) && gcd(sum + cand, (seq_t)group) == 1) {
                    val = cand;
                    break;
                }
            }
            ++term_idx;
            if (term_idx == n) return val;
            used_set(val);
            sum += val;
        }

        /* k = group: choose smallest unused multiple of group
           that makes the total sum prime. */
        seq_t val;
        for (seq_t cand = (seq_t)group; ; cand += (seq_t)group) {
            if (!used_check(cand) && is_prime_big(sum + cand)) {
                val = cand;
                break;
            }
        }
        ++term_idx;
        if (term_idx == n) return val;
        used_set(val);

        ++group;   /* advance to next group */
    }

    /* should never reach here */
    return (seq_t)0;
}
