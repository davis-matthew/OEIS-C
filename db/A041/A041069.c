/*
 * A041069  Denominators of continued fraction convergents to sqrt(41).
 * https://oeis.org/A041069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 64*a(n-3) + a(n-6) for n ≥ 6; a(0)=1, a(1)=2, a(2)=5,
 *            a(3)=62, a(4)=129, a(5)=320
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)62;
    if (n == 4) return (seq_t)129;
    if (n == 5) return (seq_t)320;

    /* Rolling buffer of the last six values */
    seq_t last[6] = {(seq_t)1, (seq_t)2, (seq_t)5, (seq_t)62, (seq_t)129, (seq_t)320};

    for (idx_t i = 6; i <= n; ++i) {
        seq_t new_val = last[3] * (seq_t)64 + last[0];
        /* shift left by one position */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = new_val;
    }
    return last[5];
}
