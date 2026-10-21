/*
 * A097300  Tenth column (m=9) of (1,6)-Pascal triangle A096956.
 * https://oeis.org/A097300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+54) * binomial(n+8, 8) / 9
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 40, u64 n <= 563,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A097300 in a safe way for 64‑bit builds. */
seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for the intermediate binomial to avoid overflow
       even when the final result still fits in 64 bits. */
    __int128 bin = 1;
    for (idx_t i = 1; i <= 8; ++i)
    {
        bin = bin * (__int128)(n + i);  /* multiply by (n+i) */
        bin /= i;                       /* divide by i, result stays integral */
    }

    __int128 res = ((__int128)n + 54) * bin; /* (n+54) * binomial(n+8,8) */
    res /= 9;                                /* divide by 9 */

    return (seq_t)res;
}
