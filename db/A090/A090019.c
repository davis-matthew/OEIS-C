/*
 * A090019  a(n) = (3*10^n + 2*0^n)/5.
 * https://oeis.org/A090019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*10^n + 2*0^n)/5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0^0 is taken as 1, so a(0)=1 and for n>0, a(n)=6*10^(n-1). */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute 10^(n-1) by repeated multiplication by 10. */
    seq_t pow10 = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i) {
        pow10 *= (seq_t)10;
    }

    /* a(n) = 6 * 10^(n-1) for n > 0 */
    return (seq_t)6 * pow10;
}
