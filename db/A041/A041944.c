/*
 * A041944  Numerators of continued fraction convergents to sqrt(495).
 * https://oeis.org/A041944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 178*a(n-2) - a(n-4) for n>=4; initial values: a(0)=22,
 *            a(1)=89, a(2)=3938, a(3)=15841
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case values */
    const seq_t a0_init = (seq_t)22;
    const seq_t a1_init = (seq_t)89;
    const seq_t a2_init = (seq_t)3938;
    const seq_t a3_init = (seq_t)15841;

    if (n == 0) return a0_init;
    if (n == 1) return a1_init;
    if (n == 2) return a2_init;
    if (n == 3) return a3_init;

    seq_t a0 = a0_init;
    seq_t a1 = a1_init;
    seq_t a2 = a2_init;
    seq_t a3 = a3_init;
    const seq_t mul = (seq_t)178;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = mul * a2 - a0;
        /* shift the window by one */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
