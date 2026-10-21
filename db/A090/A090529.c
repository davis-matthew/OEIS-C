/*
 * A090529  a(n) is the smallest positive m such that n <= m!.
 * https://oeis.org/A090529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 575, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = smallest m such that n ≤ m!  (a(0)=1) */
#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0) return (seq_t)1;

    seq_t fact = (seq_t)1;  /* 1! */
    idx_t m = 1;            /* current factorial index */

    while (1)
    {
        if (fact >= (seq_t)n)
            return (seq_t)m;  /* found minimal m */

        idx_t next = m + 1;  /* candidate m+1 */

        /* Handle possible overflow when seq_t is 64-bit */
        if (sizeof(seq_t) <= 8)  /* seq_t is uint64_t */
        {
            unsigned __int128 prod = (unsigned __int128)fact * (unsigned __int128)next;

            /* If product exceeds UINT64_MAX, the true factorial is > n */
            if (prod > (unsigned __int128)UINT64_MAX)
                return (seq_t)next;

            fact = (seq_t)prod;
        }
        else
        {
            /* For larger seq_t (uint128_t or arbitrary precision), overflow won't occur */
            fact *= next;
        }

        m = next;
    }
}
