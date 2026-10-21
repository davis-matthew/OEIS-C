/*
 * A070575  a(n) = n^4 mod 32.
 * https://oeis.org/A070575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 32
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 32.  For power‑of‑two modulus, bitwise & is equivalent to % */
    unsigned long long m = (unsigned long long)n & 31ULL;  /* m ∈ [0,31] */

    /* Compute m^2 mod 32, then square again to get m^4 mod 32 */
    unsigned long long sq     = (m * m) & 31ULL;   /* m^2 mod 32 */
    unsigned long long fourth = (sq * sq) & 31ULL;  /* m^4 mod 32 */

    return (seq_t)fourth;   /* result always < 32, fits any seq_t width */
}
