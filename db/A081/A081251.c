/*
 * A081251  Numbers k such that A081249(m)/m^2 has a local maximum for m = k.
 * https://oeis.org/A081251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:   a(1) = 2,
                  a(2) = 6,
                  a(n) = a(n-2) + 2·3^(n-1)   for n > 2
      (Equivalent closed form: a(n) = ⌊3^(n+1)/4⌋) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* out of range, offset is 1 */

    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;

    /* keep the two lagging values */
    seq_t a_nm2 = (seq_t)2;          /* a(n-2) */
    seq_t a_nm1 = (seq_t)6;          /* a(n-1) */

    /* 3^(n-1) starts as 3^2 = 9 for the first iteration (n=3) */
    seq_t pow3 = (seq_t)9;          /* 3^2 */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* a(i) = a(i-2) + 2 * 3^(i-1) */
        seq_t term = a_nm2 + (pow3 + pow3);   /* pow3 * 2 */

        /* shift the window for next step */
        a_nm2 = a_nm1;
        a_nm1 = term;

        /* prepare pow3 for the next exponent: 3^(i) = 3^(i-1) * 3 */
        pow3 *= (seq_t)3;
    }

    return a_nm1;   /* a(n) */
}
