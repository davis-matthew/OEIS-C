/*
 * A184218  a(n) = largest k such that A000217(n+1) = A000217(n) + (A000217(n)
 *          mod k), or 0 if no such k exists.
 * https://oeis.org/A184218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n-2)/2 for n >= 5, a(n) = 0 for n <= 4
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;
    /* Use seq_t arithmetic to avoid overflow of intermediate 64‑bit values */
    seq_t t1 = (seq_t)(n + 1);  /* (n+1) */
    seq_t t2 = (seq_t)(n - 2);  /* (n-2) */
    return (t1 * t2) / 2;
}
