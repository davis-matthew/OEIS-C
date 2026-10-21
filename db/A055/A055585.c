/*
 * A055585  Second column of triangle A055584.
 * https://oeis.org/A055585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 22, u64 n <= 51, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The integer formula for A055585
   a(n) = ((n+1) * (n+3) * (n+8) / 3) * 2^(n‑3)
   with division by 2^(3‑n) for n < 3. */
seq_t a(idx_t n)
{
    /* compute ((n+1)*(n+3)*(n+8)/3) */
    seq_t t = (seq_t)(n + 1);
    t *= (seq_t)(n + 3);
    t *= (seq_t)(n + 8);
    t /= (seq_t)3;                     /* exact division */

    /* multiply or divide by the appropriate power of 2 */
    if (n >= 3) {
        seq_t p = (seq_t)1;
        /* p = 2^(n-3) */
        for (idx_t i = 0; i < n - 3; ++i)
            p <<= 1;
        t *= p;
    } else {
        seq_t p = (seq_t)1;
        /* p = 2^(3-n) */
        for (idx_t i = 0; i < 3 - n; ++i)
            p <<= 1;
        t /= p;                       /* exact division */
    }
    return t;
}
