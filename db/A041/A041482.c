/*
 * A041482  Numerators of continued fraction convergents to sqrt(258).
 * https://oeis.org/A041482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(258).
   The continued fraction has period 2 after the initial term:
   a0 = 16, a_n = 16 for odd n >= 1, a_n = 32 for even n >= 2.
   Convergents numerators p_n satisfy:
     p_-2 = 0, p_-1 = 1
     p_n   = a_n * p_(n-1) + p_(n-2)
   The function returns p_n for the given index n (0‑based). */

seq_t a(idx_t n)
{
    seq_t p_nm2 = 0;  /* p_{n-2} */
    seq_t p_nm1 = 1;  /* p_{n-1} */
    seq_t p_n   = 0;  /* current numerator */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t coef = (k == 0) ? (seq_t)16
                              : ((k & 1ULL) ? (seq_t)16 : (seq_t)32);
        p_n = coef * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }

    return p_n;
}
