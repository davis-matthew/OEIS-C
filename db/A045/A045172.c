/*
 * A045172  Numbers whose base-5 representation contains exactly three 0's and
 *          three 1's.
 * https://oeis.org/A045172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose base‑5 * representation
 *            contains exactly three 0’s and exactly three 1’s. * The function
 *            builds the sequence by a linear scan with memoisation
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define CACHE_SIZE 20000      /* sufficient for n ≤ 10 000 */

seq_t a(idx_t n)
{
    static seq_t cache[CACHE_SIZE];
    static size_t  count     = 0;   /* number of terms stored      */
    static seq_t   candidate = 0;   /* next integer to test        */

    /* OEIS offset = 1; return 0 for safety if called with n < 1 */
    if (n == 0)
        return (seq_t)0;

    /* If the requested term is already cached, return it immediately. */
    if ((idx_t)count >= n)
        return cache[n - 1];

    /* Extend the cache until it contains at least n terms. */
    while ((idx_t)count < n) {
        seq_t cur = candidate;

        /* Count zeros and ones in the base‑5 representation of cur. */
        seq_t tmp   = cur;
        size_t zeros = 0, ones = 0;

        if (tmp == (seq_t)0) {
            zeros = 1;                    /* representation of 0 is "0" */
        } else {
            while (tmp > (seq_t)0) {
                seq_t d = tmp % (seq_t)5;
                if (d == (seq_t)0)
                    zeros++;
                else if (d == (seq_t)1)
                    ones++;
                tmp /= (seq_t)5;
            }
        }

        if (zeros == 3 && ones == 3) {
            if (count + 1 > CACHE_SIZE) {   /* unlikely for n ≤ 10 000 */
                /* Exceeded pre‑allocated cache – return cur (fallback). */
                return cur;
            }
            cache[count++] = cur;
        }

        candidate = cur + (seq_t)1;
    }

    /* Now the cache contains at least n terms; return the requested one. */
    return cache[n - 1];
}
