/*
 * A120370  a(1) = 2. a(n) = a(n-1) + (maximum number of distinct primes
 *          dividing any earlier terms).
 * https://oeis.org/A120370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 2; a(n) = a(n-1) + max{ ω(a(k)) | 1 ≤ k < n } where ω(m)
 *            is the number of distinct prime divisors of m. The sequence
 *            grows essentially linearly, so for n ≤ 10 000 all values fit
 *            easily into the chosen seq_t type
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* Guard against an invalid index: the specification uses only n ≥ 1. */
    if (n == 0) return (seq_t)0;

    static seq_t *buf    = NULL;   /* memoised sequence values   */
    static idx_t  len    = 0;       /* number of terms stored      */
    static int    maxomega = 0;     /* maximum ω among stored terms*/

    /* Allocate or grow the buffer if necessary. */
    if (len < n)
    {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(buf, sizeof(seq_t) * new_len);
        if (!tmp) return (seq_t)0; /* allocation failure → return 0 */
        buf = tmp;

        /* Initialise the very first term and maxomega if the buffer was empty. */
        if (len == 0)
        {
            buf[0] = (seq_t)2;
            maxomega = count_distinct_primes((uint64_t)buf[0]); /* = 1 */
            len = 1;
        }

        /* Compute the remaining terms. */
        for (idx_t i = len; i < new_len; ++i)
        {
            buf[i] = buf[i-1] + (seq_t)maxomega;
            int curomega = count_distinct_primes((uint64_t)buf[i]);
            if (curomega > maxomega) maxomega = curomega;
        }
        len = new_len;
    }

    return buf[n-1];
}
