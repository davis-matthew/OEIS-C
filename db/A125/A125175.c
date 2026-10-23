/*
 * A125175  Triangle T(n,k) = |A053123(n/2+k/2,k)| for even n+k, T(n,k)=
 *          A082985((n+k-1)/2,k) for odd n+k; read by rows, 0<=k<=n.
 * https://oeis.org/A125175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r(r+1)/2 <= n < (r+1)(r+2)/2, T(r,k) =
 *            binomial(r+1,k) if r+k is even, binomial(r-1,k)*(r+k)/(r-k) if
 *            r+k is odd
 * Range:     u8 n <= 58, u16 n <= 178, u32 n <= 612, u64 n <= 2308,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(N,K) multiplicatively, using
   128‑bit arithmetic for intermediate results to avoid overflow. */
static inline seq_t binomial_calc(idx_t N, idx_t K)
{
    if (K > N)
        return (seq_t)0;
    idx_t k = K;
    if (k > N - k)
        k = N - k;          /* symmetry: C(N,K) = C(N,N-K)   */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        unsigned __int128 mul = (unsigned __int128)res * (N - k + i);
        mul /= i;          /* exact division */
        res = (seq_t)mul;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the row. */
    idx_t first = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - first;          /* 0 <= k <= r */

    if (((r + k) & 1ULL) == 0)    /* even */
        return binomial_calc(r + 1, k);
    else                          /* odd */
    {
        seq_t val = binomial_calc(r - 1, k);
        unsigned __int128 prod = (unsigned __int128)val * (r + k);
        prod /= (r - k);          /* division gives an integer */
        return (seq_t)prod;
    }
}
