/*
 * A124099  Sum_(x^i*y^j*z^k) with i + j + k = m and (x, y, z) = the primitive
 *          Pythagorean triple (5, 12, 13).
 * https://oeis.org/A124099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A124099: a(n) = 30*a(n-1) - 281*a(n-2) + 780*a(n-3)
   with a(0)=1, a(1)=30, a(2)=619 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)30;
    if (n == 2) return (seq_t)619;

    /* Previous three values: a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)1;    /* a(0) */
    seq_t a1 = (seq_t)30;   /* a(1) */
    seq_t a2 = (seq_t)619;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 30*a(i-1) - 281*a(i-2) + 780*a(i-3) */
        seq_t next = a2 * 30;      /* 30*a(i-1) */
        next += a0 * 780;          /* + 780*a(i-3) */
        next -= a1 * 281;          /* - 281*a(i-2) */

        /* Shift window forward */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
