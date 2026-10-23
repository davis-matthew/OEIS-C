/*
 * A128012  a(n) = 3*A001399(n).
 * https://oeis.org/A128012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 3*floor((n^2+6n+12)/12)
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute p = floor((n^2+6n+12)/12) = floor(n*(n+6)/12) + 1  */
#if defined(__SIZEOF_INT128__)
    /* use 128‑bit arithmetic when available */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 prod = nn * (nn + (unsigned __int128)6);
    unsigned __int128 temp = prod / 12 + 1;      /* floor + 1   */
    seq_t p = (seq_t)temp;
#else
    /* fall back to 64‑bit arithmetic – safe for n up to ~4e9 */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long prod = nn * (nn + 6ULL);
    unsigned long long temp = prod / 12ULL + 1ULL;
    seq_t p = (seq_t)temp;
#endif
    return p * 3;
}
