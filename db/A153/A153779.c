/*
 * A153779  Numbers of the form 49*k, where 49*k+2 and 49*k-6 are both prime.
 * https://oeis.org/A153779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by searching for k such that 49*k-6 and 49*k+2
 *            are both prime
 * Range:     u8 none, u16 n <= 16, u32 n <= 24, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External utilities used */
#include <stddef.h>                 /* size_t */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n) */
#include "../utils/append_candidate.h" /* void append_candidate(seq_t **p,
                                            size_t *len, size_t *cap,
                                            seq_t val) */

/* Return the nth term of OEIS A153779 */
seq_t a(idx_t n)
{
    /* Static state for lazy computation */
    static seq_t  *seq    = NULL;   /* dynamic array of found terms */
    static size_t len    = 0;       /* number of terms stored */
    static size_t cap    = 0;       /* allocated capacity of seq */
    static idx_t  last_k = 0;       /* last k processed (starting from 0) */

    /* Ensure n >= 1 */
    if (n == 0) return (seq_t)0;    /* undefined, but avoid UB */

    /* Generate terms until we have at least n terms */
    while (len < (size_t)n) {
        /* Advance to the next candidate k */
        last_k++;

        /* Compute the candidate value 49*k */
        seq_t val = (seq_t)49 * (seq_t)last_k;

        /* Test the primality conditions */
        seq_t below = val - (seq_t)6;   /* 49*k-6 */
        seq_t above = val + (seq_t)2;   /* 49*k+2 */

        if (is_prime_big(below) && is_prime_big(above)) {
            /* Store the term */
            append_candidate(&seq, &len, &cap, val);
        }
        /* Continue until we reach the desired count */
    }

    /* Return the requested term (indexing from 1) */
    return seq[(size_t)(n - 1)];
}
