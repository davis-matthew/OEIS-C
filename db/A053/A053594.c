/*
 * A053594  Open 3-dimensional ball numbers (version 2): a(n) is the number of
 *          integer points (i,j,k) contained in an open ball of diameter n,
 *          centered at (1/2,0,0).
 * https://oeis.org/A053594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the number of integer triples (i,j,k) with (i-0.5)^2 +
 *            j^2 + k^2 < (n/2)^2. Equivalent to counting integer solutions
 *            (x,y,z) to x^2 + y^2 + z^2 < n^2 where x is odd and y,z are
 *            even. The implementation counts these by iterating over j=|y|
 *            and k=|z| and determining how many odd x satisfy t...
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n == 0) return 0;

    /* 64‑bit a^2 to avoid overflow for n up to 10⁴ */
    unsigned long long n64 = n;
    unsigned long long limit_sq = n64 * n64;          /* n^2  */
    unsigned long long max_s = (limit_sq - 1) / 4;    /* maximum j² + k² */

    unsigned long long ans = 0;

    for (unsigned long long j = 0; ; ++j)
    {
        unsigned long long j_sq = j * j;
        if (j_sq > max_s) break;
        unsigned long long weight_j = (j == 0) ? 1ULL : 2ULL;

        unsigned long long max_s_j = max_s - j_sq;
        for (unsigned long long k = 0; ; ++k)
        {
            unsigned long long k_sq = k * k;
            if (k_sq > max_s_j) break;

            unsigned long long weight = weight_j * ((k == 0) ? 1ULL : 2ULL);
            unsigned long long M = limit_sq - 4ULL * (j_sq + k_sq); /* > 0 */

            /* count odd x with x^2 < M  */
            unsigned long long R = (unsigned long long) sqrt((double)(M - 1ULL));
            unsigned long long count_odd = 2ULL * ((R + 1ULL) / 2ULL);

            ans += weight * count_odd;
        }
    }

    return (seq_t) ans;
}
