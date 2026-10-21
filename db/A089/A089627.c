/*
 * A089627  T(n,k) = binomial(n,2*k)*binomial(2*k,k) for 0 <= k <= n, triangle
 *          read by rows.
 * https://oeis.org/A089627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 94, u32 n <= 306, u64 n <= 1047,
 *            u128 n <= 3682, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = binomial(r, 2k) * binomial(2k, k)
 * where r and k are obtained from the index n by mapping it into the
 * triangular array read by rows:
 *   r = floor( (sqrt(8*n + 1) - 1) / 2 )
 *   k = n - r*(r+1)/2
 *
 * If 2k > r then binomial(r, 2k) is zero and a(n) = 0.
 */
#include <stdint.h>

/* Assumes seq_t and idx_t are defined elsewhere in the build. */

static seq_t binomial_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (n - k + i);
        res = res / i;            /* division is exact for binomial */
    }
    return res;
}

static idx_t triangular_row(idx_t n)
{
    /* Solve r(r+1)/2 <= n < (r+1)(r+2)/2 */
    unsigned __int128 t = (unsigned __int128)8 * n + 1;
    unsigned __int128 s = 0;
    /* integer square root of t */
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64) - 1;
    while (low <= high)
    {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 mid2 = mid * mid;
        if (mid2 <= t) { s = mid; low = mid + 1; }
        else high = mid - 1;
    }
    idx_t r = (idx_t)((s - 1) / 2);
    /* Correct possible off‑by‑one due to floor sqrt */
    while ((unsigned __int128)r * (r + 1) / 2 > n) --r;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) ++r;
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = triangular_row(n);
    idx_t start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - start;

    idx_t two_k = k + k;
    if (two_k > r) return 0;

    seq_t part1 = binomial_idx(r, two_k);
    seq_t part2 = binomial_idx(two_k, k);

    return part1 * part2;
}
