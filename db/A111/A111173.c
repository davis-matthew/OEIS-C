/*
 * A111173  Sophie Germain triprimes: k and 2k + 1 are both the product of 3
 *          primes, not necessarily distinct.
 * https://oeis.org/A111173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega_uint64, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A111173: a(n) is the n‑th integer k such that
 * k is a product of three primes (not necessarily distinct) and
 * 2k+1 is also a product of three primes.  The function searches
 * integers in ascending order, keeping a static cache so that
 * subsequent calls are fast, no matter the order of n. */

#include <stdint.h>
#include "../utils/bigomega_uint64.h"
#include "../utils/push_seq.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *arr = NULL;            /* array of seen triprimes    */
    static idx_t   cap = 0;              /* current allocation size    */
    static idx_t   size = 0;             /* number of terms stored     */
    static uint64_t last = 0;            /* last integer examined      */

    if (n == 0)                 /* the offset is 1; undefined for 0 */
        return 0;

    /* generate new terms until we have at least n entries */
    while (size < n) {
        last++;                         /* move to next integer          */

        /* check if last is a product of exactly 3 primes */
        if (bigomega_uint64((uint64_t)last) != 3)
            continue;

        /* check 2k+1 */
        uint64_t two_k_plus1 = 2ULL * last + 1ULL;
        if (bigomega_uint64(two_k_plus1) != 3)
            continue;

        /* store the valid term */
        push_seq(&arr, &cap, &size, (seq_t)last);
    }

    /* return the requested term (1‑based index) */
    return arr[n - 1];
}
