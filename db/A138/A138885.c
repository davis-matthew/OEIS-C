/*
 * A138885  n-th run has length n-th nonprime number, with digits 0 and 1
 *          only, starting with 1.
 * https://oeis.org/A138885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138885: runs of 1 and 0, run lengths are the
   non‑prime numbers (1,4,6,8,9,…), starting with 1. */
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t); */

/* Return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)          /* out of offset – harmless */
        return (seq_t)0;

    idx_t pos = n;          /* target position */
    idx_t cumulative = 0;   /* sum of run lengths seen so far */
    idx_t run_index = 0;    /* 1‑based run counter */
    idx_t candidate = 1;    /* next integer to test as a non‑prime length */

    while (cumulative < pos) {
        /* Find the next non‑prime integer.
           Non‑prime includes 0,1 and every composite number.  */
        while (candidate > 1 && is_prime(candidate))
            ++candidate;

        /* candidate is now a non‑prime number (1,4,6,…) */
        cumulative += candidate;   /* add its run length */
        ++run_index;               /* advance to next run */
        ++candidate;               /* next integer for the following run */
    }

    /* Runs start with 1 at index 1, 0 at index 2, alternately. */
    return (run_index & 1) ? (seq_t)1 : (seq_t)0;
}
