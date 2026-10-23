/*
 * A101330  Array read by antidiagonals: T(n, k) = Knuth's Fibonacci (or
 *          circle) product of n and k ("n o k"), n >= 1, k >= 1.
 * https://oeis.org/A101330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) is the nth entry read by antidiagonals from the array
 *            T(n,k) = 3*n*k - n*floor((k+1)/phi²) - k*floor((n+1)/phi²) where
 *            phi² = (3+√5)/2 ≈ 2.61803398875. The floor term is computed
 *            using the constant 1/phi² ≈ 0.3819660112501051
 * Range:     u8 n <= 218, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const long double INV_PHI2 = 0.3819660112501051L;

/* floor((m+1)/phi²) using a high‑precision approximation of 1/phi². */
static inline idx_t h_const(idx_t m)
{
    return (idx_t)((long double)(m + 1) * INV_PHI2);
}

/* Knuth's Fibonacci (circle) product n o k */
static inline seq_t T_product(idx_t n, idx_t k)
{
    seq_t ns = (seq_t)n;
    seq_t ks = (seq_t)k;
    seq_t hn = (seq_t)h_const(n);
    seq_t hk = (seq_t)h_const(k);
    seq_t prod = 3 * ns * ks;
    prod -= ns * hk;
    prod -= ks * hn;
    return prod;
}

/* a(n) : read the array by antidiagonals */
seq_t a(idx_t n)
{
    /* find diagonal d such that d*(d+1)/2 >= n */
    idx_t d = 1;
    while (( (__int128)d * (d + 1) / 2 ) < n) d++;

    /* first index in this diagonal */
    idx_t start = ( (__int128)(d - 1) * d / 2 ) + 1;
    idx_t p = n - start;        /* 0‑based offset within diagonal */

    idx_t i = p + 1;            /* row index (n in T)   */
    idx_t j = d - p;            /* column index (k)      */

    return T_product(i, j);
}
