/*
 * A123023  a(n) = (n-1)*a(n-2), a(0)=1, a(1)=0.
 * https://oeis.org/A123023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*a(n-2), a(0)=1, a(1)=0
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 35, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)0; /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ai = (seq_t)(i - 1) * a0; /* a(i) = (i-1)*a(i-2) */
        a0 = a1;
        a1 = ai;
    }

    return a1;
}
