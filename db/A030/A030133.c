/*
 * A030133  a(n+1) is the sum of digits of (a(n) + a(n-1)).
 * https://oeis.org/A030133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   a(0)=2, a(1)=1, a(n+1)=digit_sum( a(n)+a(n-1) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9067 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* digit_sum(s) = s               if s < 10
 *             = s - 9           if s >= 10   (since a(n) and a(n-1) are single digits) */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* cache of computed values */
    static idx_t  len  = 0;     /* current length of cache */

    if (n < 2) {
        return (n == 0) ? (seq_t)2 : (seq_t)1;
    }

    /* initialise cache for the first call */
    if (!seq) {
        len = 2;
        seq = mem_new(seq_t, len);
        if (!seq) return 0;         /* failure */
        seq[0] = (seq_t)2;
        seq[1] = (seq_t)1;
    }

    /* extend cache up to n if needed */
    if (n >= len) {
        seq = mem_resize(seq_t, seq, (n + 1));
        if (!seq) return 0;         /* failure */
        for (idx_t i = len; i <= n; ++i) {
            seq_t sum = seq[i - 1] + seq[i - 2]; /* sum <= 18 */
            seq[i] = (sum > 9) ? (sum - 9) : sum;
        }
        len = n + 1;
    }

    return seq[n];
}
