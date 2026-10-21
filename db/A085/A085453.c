/*
 * A085453  Numbers n such that n^2 and n^3 together use only distinct digits.
 * https://oeis.org/A085453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = the nth integer whose square and cube together * use only
 *            distinct decimal digits (each digit may appear at most once *
 *            across n² and n³). The sequence is finite in base 10 (six terms)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   distinct_digits_across
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation scans positive integers until the required
 * number of solutions is found, memoises them in a static array,
 * and then serves any subsequent request in O(1). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/distinct_digits_across.h"

seq_t a(idx_t n)
{
    /* Memoised list of found numbers. The list is generated on the
     * first call; thereafter a simple lookup is performed. */
    static seq_t *list = NULL;       /* list[0] = a(1), etc. */
    static size_t count = 0;         /* number of valid entries discovered */
    static size_t capacity = 0;      /* allocated size of list */
    static bool initialized = false;

    if (!initialized) {
        /* Reserve space for a few terms; the sequence has only six. */
        capacity = 8;
        list = (seq_t*)malloc(sizeof(seq_t) * capacity);
        count = 0;

        /* Brute‑force search: increment candidate, test, store. */
        for (idx_t cand = 1; count < capacity && cand <= 1000000; ++cand) {
            /* Compute n² and n³ in seq_t. Make sure candidate is converted
               to seq_t before multiplication to avoid overflow. */
            seq_t sq = (seq_t)cand * (seq_t)cand;
            seq_t cube = sq * (seq_t)cand;

            if (distinct_digits_across(sq, cube)) {
                /* Store the candidate. */
                list[count++] = (seq_t)cand;
            }
        }

        /* The sequence in base 10 is known to contain only six numbers.
           If we didn't discover them all (unlikely), we trust the
           above loop to have found them. */
        initialized = true;
    }

    if (n == 0 || n > count) {
        /* Undefined for the requested index (e.g., out of range). */
        return 0;
    }
    return list[n - 1];   /* a(1) is at list[0] */
}
