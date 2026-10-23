/*
 * A041888  Numerators of continued fraction convergents to sqrt(466).
 * https://oeis.org/A041888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to √466.
   Recurrence:
     let p[-2] = 0, p[-1] = 1
     a(0) = 21
     for n ≥ 1 let c(n) = period[(n‑1) mod 22] where
         period = {1,1,2,2,1,2,5,1,3,1,20,1,3,1,5,2,1,2,2,1,1,42}
     then a(n) = c(n) * p[n‑1] + p[n‑2]. */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)21;

    static const idx_t period[22] = {
        1, 1, 2, 2, 1, 2, 5, 1, 3, 1,
        20, 1, 3, 1, 5, 2, 1, 2, 2, 1,
        1, 42
    };
    const idx_t period_len = 22;

    seq_t p_im2 = (seq_t)0;   /* p[-2] */
    seq_t p_im1 = (seq_t)1;   /* p[-1] */
    seq_t p_val  = 0;         /* p[n] */

    for (idx_t k = (idx_t)0; k <= n; ++k)
    {
        seq_t coeff;
        if (k == (idx_t)0)
            coeff = (seq_t)21;
        else
            coeff = (seq_t)period[(k - 1) % period_len];

        p_val = coeff * p_im1 + p_im2;

        p_im2 = p_im1;
        p_im1 = p_val;
    }

    return p_val;
}
