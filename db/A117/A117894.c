/*
 * A117894  Triangle, row sums = Pell numbers, A000129.
 * https://oeis.org/A117894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 118, u32 n <= 403, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A117894 – Flattened triangle with entries T(r,k) defined by
 *  T(n,0)=1, and for 1 <= k <= n:
 *
 *      T(n,k) = (3*(n-k)+1) * P_k  –  (n-k) * P_{k+1}
 *
 *  where P_m is the m-th Pell number (P_0=0, P_1=1,
 *  P_{m+2}=2*P_{m+1}+P_m).  The triangle rows are 
 *  concatenated to form the sequence a(n).  This function
 *  computes a(n) for any n >= 0, using only integer
 *  arithmetic (no floating‑point operations on seq_t).
 *
 *  The implementation caches Pell numbers across calls,
 *  extending the cache lazily as needed.
 */

#include <stdlib.h>
#include <math.h>

/* Integer square‑root: returns floor(sqrt(x)) for uint64_t x. */
static idx_t isqrt_u64(idx_t x)
{
    /* Upper bound for sqrt(8*n+1) fits in 32 bits for all n < 2^64. */
    idx_t lo = 0;
    idx_t hi = 4294967296ULL;          /* 2^32 */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Compute a(n) – the flattened triangle entry. */
seq_t a(idx_t n)
{
    /* Locate row r and column c (0‑based).  Row r satisfies
     *   r(r+1)/2 <= n < (r+1)(r+2)/2
     *   r = floor((sqrt(8*n+1)-1)/2)
     */
    long double sqrt_term = sqrtl((long double)8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((sqrt_term - 1.0L) * 0.5L);
    idx_t first_in_row = (r * (r + 1)) / 2;
    idx_t k = n - first_in_row;              /* column */

    /* Special case: first entry in every row is 1. */
    if (k == 0)
        return (seq_t)1;

    /* Ensure Pell numbers are cached up to index k+1. */
    static seq_t *pell = NULL;
    static idx_t pell_len = 0;               /* number of stored elements */

    if (pell == NULL) {
        pell_len = 2;                        /* store P0 and P1 */
        pell = (seq_t *)malloc(pell_len * sizeof(seq_t));
        if (!pell)
            abort();
        pell[0] = (seq_t)0;                 /* P0 */
        pell[1] = (seq_t)1;                 /* P1 */
    }

    idx_t need = k + 1;                      /* we need Pell[need] */
    if (need >= pell_len) {
        size_t new_len = (size_t)need + 1;
        seq_t *tmp = (seq_t *)realloc(pell, new_len * sizeof(seq_t));
        if (!tmp)
            abort();
        pell = tmp;
        for (idx_t i = pell_len; i <= need; ++i)
            pell[i] = (seq_t)2 * pell[i - 1] + pell[i - 2];
        pell_len = new_len;
    }

    /* Compute T(r,k) without signed arithmetic on seq_t. */
    idx_t diff = r - k;                      /* n - k, non‑negative */
    seq_t coeff  = (seq_t)(3 * diff + 1);    /* 3*(n-k)+1 */
    seq_t term2  = coeff * pell[k];          /* (3*(n-k)+1)*P_k   */
    seq_t term1  = (seq_t)diff * pell[k + 1]; /* (n-k)*P_{k+1}     */

    return term2 - term1;
}
