/*
 * A193858  Triangular array: the fission of ((x+1)^n) by ((2x+1)^n).
 * https://oeis.org/A193858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{j=0..k} 2^(r-j) * binomial(r-j, k-j) where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2 (Triangular
 *            indexing of the array T(n,k))
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 161, u64 n <= 607,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------------------------------------------------------------ */
/* Integer square‑root for 128‑bit unsigned values              */
/* ------------------------------------------------------------ */
static idx_t isqrt128(unsigned __int128 v)
{
    idx_t low = 0;
    /* high bound >> 2 of max 8*n+1 for n up to 2^64 → < 2^36          */
    idx_t high = (idx_t)1 << 36;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;          /* floor sqrt(v) */
}

/* ------------------------------------------------------------ */
/*  Powers of two (safe for arbitrary seq_t)                    */
/* ------------------------------------------------------------ */
static seq_t pow2_idx(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r = r * 2;
    return r;
}

/* ------------------------------------------------------------ */
/*  Binomial coefficient C(n,k) with multiplicative algorithm  */
/* ------------------------------------------------------------ */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;               /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;   /* division is exact in the integer sense */
    }
    return res;
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Compute r = floor((sqrt(8*n+1)-1)/2)   */
    unsigned __int128 val = ((unsigned __int128)n << 3) + 1;  /* 8*n + 1   */
    idx_t r = (isqrt128(val) - 1) >> 1;                     /* divide by 2 */

    /* offset = r*(r+1)/2  <==> first index of row r               */
    idx_t offset = (idx_t)(((__uint128_t)r * (r + 1)) >> 1);

    idx_t k = n - offset;           /* column index within the row      */

    seq_t sum = 0;
    for (idx_t j = 0; j <= k; ++j)
    {
        idx_t n_j = r - j;
        idx_t k_j = k - j;
        seq_t term = pow2_idx(n_j) * binom_idx(n_j, k_j);
        sum += term;
    }
    return sum;
}
