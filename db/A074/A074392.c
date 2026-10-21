/*
 * A074392  a(n) = Lucas(n+1) + (3*(-1)^n - 1)/2.
 * https://oeis.org/A074392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n+1) + (3*(-1)^n - 1)/2 Recurrence: a(n) = a(n-1) +
 *            2*a(n-2) - a(n-3) - a(n-4) The implementation uses the
 *            definition above: a(n) = Lucas(n+1) + (1 if n even, -2 if n odd)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compute Lucas(n+1) iteratively */
    idx_t k = n + 1;                /* we need L(k), where k >= 1 for n >= 0 */
    seq_t l0 = (seq_t)2;            /* L(0) */
    seq_t l1 = (seq_t)1;            /* L(1) */

    if (k == 0) {                   /* not reached for n >= 0, but kept for completeness */
        seq_t res = l0 + ((n & 1) ? (seq_t)(-2) : (seq_t)1);
        return res;
    }

    if (k == 1) {                   /* L(1) = 1 */
        seq_t lucas = l1;
        return (n & 1) ? (lucas - (seq_t)2) : (lucas + (seq_t)1);
    }

    /* For k >= 2 compute L(k) using L(i) = L(i-1) + L(i-2) */
    for (idx_t i = 2; i <= k; ++i) {
        seq_t l2 = l0 + l1;
        l0 = l1;
        l1 = l2;
    }
    seq_t lucas = l1;                /* Now l1 holds L(k) */

    return (n & 1) ? (lucas - (seq_t)2) : (lucas + (seq_t)1);
}
