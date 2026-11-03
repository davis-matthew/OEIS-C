/*
 * A181132  a(0)=0; thereafter a(n) = total number of 0's in binary expansions
 *          of 1, ..., n.
 * https://oeis.org/A181132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) computed via the recurrence a(0) = 0, a(1) = 0, a(2n) =
 *            a(n) + a(n-1) + n, a(2n+1) = 2*a(n) + n This recurrence is
 *            equivalent to counting the total number of 0's in the binary
 *            expansions of 1, …, n
 * Range:     u8 n <= 98, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 83 terms through n = 933 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t compute_a_rec(idx_t n)
{
    if (n <= 1) return 0;                       /* a(0)=a(1)=0 */
    if (n & 1ULL) {                              /* odd: a(2m+1) */
        idx_t m = (n - 1) >> 1;                  /* m = (n-1)/2 */
        seq_t v = compute_a_rec(m);
        return v * 2 + (seq_t)m;
    } else {                                     /* even: a(2m) */
        idx_t m = n >> 1;                        /* m = n/2 */
        seq_t v1 = compute_a_rec(m);
        seq_t v2 = compute_a_rec(m - 1);
        return v1 + v2 + (seq_t)m;
    }
}

seq_t a(idx_t n)
{
    return compute_a_rec(n);
}
