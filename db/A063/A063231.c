/*
 * A063231  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          75 ).
 * https://oeis.org/A063231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A063231: Dimension of the space of weight 2n cuspidal newforms for Gamma0(75)
   a(1) = 3
   For n ≥ 2, let r = (n‑1) mod 6 and q = floor((n‑1)/6).
   Then
   a(n) = 3 + 38·q + {0, 7, 12, 20, 25, 32}[r].
   Equivalent to the recurrence
   a(1)=3, a(n)=a(n‑1)+{7,5,8,5,7,6}[ (n‑1) mod 6 ] for n≥2.   */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not part of the OEIS offset, but harmless */
    /* add[r] gives the cumulative sum of the first r differences in one period */
    static const seq_t add[6] = {0, 7, 12, 20, 25, 32};

    idx_t q = (n - 1) / 6;      /* number of full 6‑term periods before n */
    idx_t r = (n - 1) % 6;      /* position inside the current period */

    seq_t result = (seq_t)3;                 /* a(1) */
    result += (seq_t)q * (seq_t)38;          /* 38 per full period (7+5+8+5+7+6) */
    result += add[r];                        /* add the partial sum for the tail */
    return result;
}
