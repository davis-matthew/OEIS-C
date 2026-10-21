/*
 * A073659  a(1) = 1; for n > 1, a(n) is the smallest even number not already
 *          in the sequence such that a(1) + ... + a(n) is a prime.
 * https://oeis.org/A073659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 442, u32 n <= 66, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A073659
 *  a(1) = 1; for n > 1, a(n) is the smallest even number not already in the sequence
 *  such that the cumulative sum a(1)+…+a(n) is prime.
 *  The function is memoised; it works for any build where seq_t and idx_t exist,
 *  and uses the provided is_prime_big helper (fast primality test for seq_t). */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n=0, but safe */

    /* Static state for memoisation */
    static seq_t *seq          = NULL;  /* array of sequence terms */
    static size_t seq_cap      = 0;     /* capacity of seq array      */
    static size_t seq_len      = 0;     /* number of terms computed   */
    static seq_t sum           = 0;     /* cumulative sum             */

    static bool *used          = NULL;  /* flag array of even numbers used (index = e/2) */
    static size_t used_cap     = 0;     /* capacity of used array      */

    static seq_t min_unused = 2;        /* smallest even number still unused */

    /* --- initialise first term --- */
    if (seq_len == 0) {
        /* allocate capacity for first term */
        seq_cap = 1;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();
        seq[0] = (seq_t)1;   /* a(1) = 1 */
        seq_len = 1;
        sum = seq[0];
    }

    /* --- generate terms up to n --- */
    while (seq_len < (size_t)n) {
        seq_t candidate = min_unused;

        /* Find the next unused even candidate that yields a prime sum */
        for (;;) {
            /* Ensure used array can hold candidate */
            size_t idx = (size_t)(candidate >> 1);   /* even numbers only */
            if (idx >= used_cap) {
                size_t new_cap = idx + 1;
                bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
                if (!tmp) abort();
                /* initialise new part to false */
                for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
                used = tmp;
                used_cap = new_cap;
            }

            if (used[idx]) {                /* already used, jump to next even */
                candidate += (seq_t)2;
                continue;
            }

            seq_t new_sum = sum + candidate;
            if (is_prime_big(new_sum)) {    /* found the term */
                /* Grow seq array if necessary */
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap ? seq_cap * 2 : 1;
                    seq_t *tmp = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    seq = tmp;
                }
                seq[seq_len++] = candidate;
                sum = new_sum;
                used[idx] = true;

                /* Update min_unused: it stays the same if it is still unused,
                 * otherwise advance to the next unused even number. */
                if (candidate == min_unused) {
                    do {
                        min_unused += (seq_t)2;
                        size_t new_idx = (size_t)(min_unused >> 1);
                        if (new_idx >= used_cap) {
                            size_t new_cap = new_idx + 1;
                            bool *tmp2 = (bool *)realloc(used, new_cap * sizeof(bool));
                            if (!tmp2) abort();
                            for (size_t i = used_cap; i < new_cap; ++i) tmp2[i] = false;
                            used = tmp2;
                            used_cap = new_cap;
                        }
                    } while (used[(size_t)(min_unused >> 1)]);
                }
                /* term found, move to next n */
                break;
            } else {                           /* sum not prime, try next even */
                candidate += (seq_t)2;
            }
        }
    }

    return seq[n - 1];
}
