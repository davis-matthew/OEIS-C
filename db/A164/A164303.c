/*
 * A164303  a(n) = 2*a(n-1) + a(n-2) for n > 1; a(0) = 3, a(1) = 11.
 * https://oeis.org/A164303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) for n > 1; a(0) = 3, a(1) = 11
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 3;
    if (n == 1)
        return 11;

    seq_t a0 = 3;      /* a(n-2) */
    seq_t a1 = 11;     /* a(n-1) */
    seq_t an = 0;      /* current a(n) */
    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * (seq_t)2 + a0; /* a(i) = 2*a(i-1)+a(i-2) */
        a0 = a1;
        a1 = an;
    }
    return an;
}
