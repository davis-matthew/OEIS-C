/*
 * A171512  a(n) = numbers m such that are not the sum of k-th nonprime number
 *          and k for any k >= 1.
 * https://oeis.org/A171512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A171512: a(n) = n‑th positive integer NOT equal to (k‑th nonprime)+k
   for any k ≥ 1.  The sequence is the complement of the set
   {A018252(k)+k}. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Handle zero or negative indices trivially – the OEIS offset is 1. */
    if (n == 0) return (seq_t)0;

    /* Integer being examined. */
    idx_t m = 1;

    /* Current index of the nonprime sequence. */
    idx_t k = 1;

    /* Current k‑th nonprime number. */
    idx_t current_nonprime = 1;   /* A018252(1) = 1 */

    /* Value of the excluded set element s_k = nonprime(k)+k. */
    idx_t current_s = current_nonprime + k;  /* 2 */

    /* Candidate number for the next nonprime.  Starts just below the first
       nonprime so that the first advancement reaches 1. */
    idx_t candidate = 1;

    /* Count of missing numbers found so far. */
    idx_t count = 0;

    /* Iterate over natural numbers until the nth missing number is located. */
    while (1) {
        if (m == current_s) {
            /* m belongs to the excluded set; skip it. */

            /* Advance to the next nonprime. */
            k++;
            do {
                candidate++;
            } while (candidate <= UINT64_MAX && is_prime(candidate));
            current_nonprime = candidate;

            /* Compute the next excluded value. */
            current_s = current_nonprime + k;
        } else {
            /* m is not excluded; it is part of our sequence. */
            count++;
            if (count == n) {
                /* The nth such number has been found. */
                return (seq_t)m;
            }
        }
        /* Move to the next integer. */
        m++;
    }
}
