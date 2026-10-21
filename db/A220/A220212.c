/*
 * A220212  Convolution of natural numbers (A000027) with tetradecagonal
 *          numbers (A051866).
 * https://oeis.org/A220212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(3*n-2)/6
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 303, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* Use 128‑bit intermediate to avoid overflow on 64‑bit seq_t. */
    unsigned __int128 prod = (unsigned __int128)n
                         * (n + 1)
                         * (n + 2)
                         * ((unsigned __int128)3 * n - 2);
    prod /= 6;
    return (seq_t)prod;
}
