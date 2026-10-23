/*
 * A178832  Number of minima of the 1-D Griewank function in [-n, n].
 * https://oeis.org/A178832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2 * floor(n / (2π))
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 2π ≈ 6.283185307179586, hence 1/(2π) ≈ 0.159154943092.  */
seq_t a(idx_t n)
{
    /* Use a rational approximation of 1/(2π) with sufficient precision
       so that floor(n * 1/(2π)) is exact for all n ≤ 10⁴. */
    const idx_t NUM = 159154943092ULL;          /* numerator */
    const idx_t DEN = 1000000000000ULL;          /* denominator (1e12) */

    /* Compute k = floor(n * NUM / DEN) using 128‑bit arithmetic to avoid overflow. */
    __int128 mul = ( __int128 ) n * NUM;
    idx_t k = (idx_t)( mul / DEN );

    return (seq_t)(1 + 2 * k);
}
