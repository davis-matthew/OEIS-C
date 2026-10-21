/*
 * A067183  Numbers k such that the product of the prime factors of k equals
 *          the product of the digits of k.
 * https://oeis.org/A067183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A067183:  numbers k such that the product of the distinct prime
   factors of k equals the product of the decimal digits of k.
   The sequence consists of 7‑smooth numbers without zero digits.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Return the n‑th term of A067183 (1‑based). */
seq_t a(idx_t n)
{
    /* --- static storage ---------------------------------------- */
    static seq_t *humbles = NULL;     /* all 7‑smooth numbers generated so far */
    static size_t  humbles_len  = 0;
    static size_t  humbles_cap  = 0;

    static seq_t *results  = NULL;    /* sequence terms found so far */
    static size_t  results_len = 0;
    static size_t  results_cap = 0;

    static size_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;   /* indices for the
                                                       Hamming generator */

    /* --- initialise on first call -------------------------------- */
    if (humbles == NULL) {
        humbles_cap = 16;
        humbles = (seq_t *)malloc(humbles_cap * sizeof(seq_t));
        humbles[0] = 1;
        humbles_len = 1;

        results_cap = 16;
        results = (seq_t *)malloc(results_cap * sizeof(seq_t));
        results[0] = 1;            /* 1 satisfies the condition */
        results_len = 1;
    }

    /* --- generate terms until we have at least n of them ------------- */
    while (results_len < (size_t)n) {
        /* make sure the humbles array can hold the next element   */
        if (humbles_len == humbles_cap) {
            humbles_cap *= 2;
            humbles = (seq_t *)realloc(humbles,
                                       humbles_cap * sizeof(seq_t));
        }

        /* next 7‑smooth numbers coming from 2,3,5,7 */
        uint64_t next2 = 2ULL * (uint64_t)humbles[i2];
        uint64_t next3 = 3ULL * (uint64_t)humbles[i3];
        uint64_t next5 = 5ULL * (uint64_t)humbles[i5];
        uint64_t next7 = 7ULL * (uint64_t)humbles[i7];

        /* select the smallest of the four candidates */
        uint64_t next = next2;
        if (next3 < next) next = next3;
        if (next5 < next) next = next5;
        if (next7 < next) next = next7;

        /* store the new 7‑smooth number */
        humbles[humbles_len++] = (seq_t)next;

        /* advance the indices that produced the minimum */
        if (next == next2) i2++;
        if (next == next3) i3++;
        if (next == next5) i5++;
        if (next == next7) i7++;

        /* ----- test the condition ----------------------------------- */
        /* product of distinct prime factors (rad(k)) */
        uint64_t rad = 1;
        if (next % 2ULL == 0ULL) rad *= 2;
        if (next % 3ULL == 0ULL) rad *= 3;
        if (next % 5ULL == 0ULL) rad *= 5;
        if (next % 7ULL == 0ULL) rad *= 7;

        /* product of decimal digits of next, stop early if it already
           exceeds the possible maximum (210).                          */
        uint64_t prod = 1;
        uint64_t temp = next;
        while (temp > 0ULL) {
            uint64_t d = temp % 10ULL;
            prod *= d;
            if (prod > 210ULL) break;   /* no need to keep going */
            temp /= 10ULL;
        }

        if (prod == rad) {
            /* make sure the results array can hold the new term   */
            if (results_len == results_cap) {
                results_cap *= 2;
                results = (seq_t *)realloc(results,
                                            results_cap * sizeof(seq_t));
            }
            results[results_len++] = (seq_t)next;
        }
    }

    /* return the requested element (1‑based) */
    return results[(size_t)n - 1];
}
