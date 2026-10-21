/*
 * A051630  Poincaré series [or Poincare series] (or Molien series) for
 *          Gamma_2(1,2)_(2).
 * https://oeis.org/A051630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = floor((2*n^3 + 21*n^2 + 90*n + 216)/216 * - (n mod 2)*n/6
 *            * - ((2*n^2 + 2*n) mod 3)*(n+6)/18)
 * Range:     u8 n <= 26, u16 n <= 188, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider integer type for the intermediate arithmetic
       to avoid overflow for the tested range (n up to ~10000). */
#if defined(__SIZEOF_INT128__)
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 num   = 2*nn*nn*nn + 21*nn*nn + 90*nn + 216;
    unsigned __int128 sub1  = (nn & 1) * nn * 36;
    unsigned __int128 temp  = 2*nn*nn + 2*nn;
    unsigned __int128 mod3  = temp % 3;
    unsigned __int128 sub2  = mod3 * (nn + 6) * 12;
    unsigned __int128 result = (num - sub1 - sub2) / 216;
    return (seq_t)result;
#else
    unsigned long long nn = (unsigned long long)n;
    unsigned long long num   = 2ULL*nn*nn*nn + 21ULL*nn*nn + 90ULL*nn + 216ULL;
    unsigned long long sub1  = (nn & 1ULL) * nn * 36ULL;
    unsigned long long temp  = 2ULL*nn*nn + 2ULL*nn;
    unsigned long long mod3  = temp % 3ULL;
    unsigned long long sub2  = mod3 * (nn + 6ULL) * 12ULL;
    unsigned long long result = (num - sub1 - sub2) / 216ULL;
    return (seq_t)result;
#endif
}
