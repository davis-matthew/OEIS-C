/*
 * A064306  Convolution of A052701 (Catalan numbers multiplied by powers of 2)
 *          with powers of -1.
 * https://oeis.org/A064306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -a(n-1) + Catalan(n) * 2^n, a(-1) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prev = 0;          /* a(-1) */
    seq_t cat  = 1;          /* Catalan(0) */
    seq_t pow2 = 1;          /* 2^0 */

    for (idx_t i = 0; i <= n; ++i)
    {
        if (i > 0)
        {
            /* Catalan recurrence: C_i = C_{i-1} * (4*i - 2) / (i + 1) */
            cat = cat * (seq_t)(4 * i - 2) / (idx_t)(i + 1);
            pow2 <<= 1;                 /* 2^i */
        }

        seq_t term = cat * pow2;        /* Catalan(i) * 2^i */
        seq_t curr = term - prev;       /* term - a(i-1) == -a(i-1) + term  */
        prev = curr;                    /* a(i) */
    }

    return prev;
}
