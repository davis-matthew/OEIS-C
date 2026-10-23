/*
 * A195149  Concentric 22-gonal numbers.
 * https://oeis.org/A195149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (22*n^2 + 9*(-1)^n - 9)/4
 * Range:     u8 n <= 6, u16 n <= 109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic when available to avoid overflow for large n. */
#if defined(__SIZEOF_INT128__)
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 val = 22 * nn * nn;
    if (n & 1ULL)
        val -= 18;
    val /= 4;
    return (seq_t)val;
#else
    /* Fallback: 64‑bit arithmetic.  This is adequate for the test range
       (n up to ~10 000), where 22*n*n <= 2.2×10⁹. */
    unsigned long long nn = n;
    unsigned long long val = 22ULL * nn * nn;
    if (n & 1ULL)
        val -= 18ULL;
    val /= 4ULL;
    return (seq_t)val;
#endif
}
