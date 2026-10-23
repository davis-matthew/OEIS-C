/*
 * A017901  Expansion of 1/(1 - x^7 - x^8 - ...).
 * https://oeis.org/A017901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 0 for 1 ≤ n ≤ 6; For n ≥ 7: a(n) = a(n-1) +
 *            a(n-7)
 * Range:     u8 n <= 34, u16 n <= 59, u32 n <= 108, u64 n <= 205,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of A017901.  */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first seven values directly.  */
    if (n == 0)
        return (seq_t)1;
    if (n < 7)
        return (seq_t)0;

    /* A small circular buffer holds the last seven values.  */
    seq_t buf[7] = {
        (seq_t)1, /* a(0) */
        (seq_t)0, /* a(1) */
        (seq_t)0, /* a(2) */
        (seq_t)0, /* a(3) */
        (seq_t)0, /* a(4) */
        (seq_t)0, /* a(5) */
        (seq_t)0  /* a(6) */
    };

    /* Iterate from 7 up to n, updating the buffer.  */
    for (idx_t k = 7; k <= n; ++k) {
        /* (k-1) and (k-7) are always non‑negative for k≥7.  */
        seq_t val = buf[(k - 1) % 7] + buf[(k - 7) % 7];
        buf[k % 7] = val;
    }

    return buf[n % 7];
}
