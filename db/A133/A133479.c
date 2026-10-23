/*
 * A133479  a(n) = a(n-1) + 8*a(n-2) for n >= 2, a(0)=1, a(1)=2.
 * https://oeis.org/A133479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 8*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first two base cases directly */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)2;
    }

    /* iterative computation for n >= 2 */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)2;   /* a(1) */
    seq_t an;              /* temporary storage for a(i) */

    for (idx_t i = 2; i <= n; ++i) {
        an = a1 + (seq_t)8 * a0;
        a0 = a1;
        a1 = an;
    }

    return a1;  /* a(n) is stored in a1 after the loop */
}
