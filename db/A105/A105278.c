/*
 * A105278  Triangle read by rows: T(n,k) = binomial(n,k)*(n-1)!/(k-1)!.
 * https://oeis.org/A105278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 68, u64 n <= 191,
 *            u128 n <= 530, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: T(n,k) = C(n,k)⋅(n‑1)!/(k‑1)!.
   The sequence a(n) is the concatenation of the rows r=1,2,…,
   i.e. a(n)=T(r,k) where r is the least integer with r(r+1)/2≥n
   and k=n−r(r−1)/2. */
#include <stdint.h>

/* Compute binomial coefficient C(N,K) using a multiplicative
   method so that only integer arithmetic on seq_t is required. */
static seq_t binom_uint64(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K == 0 || K == N) return 1;
    if (K > N - K) K = N - K;          /* use symmetry */

    seq_t result = 1;
    /* multiply and divide in each step to keep the intermediate
       within seq_t range (it always divides evenly). */
    for (idx_t i = 1; i <= K; ++i) {
        result = result * (N - K + i);
        result = result / i;
    }
    return result;
}

/* Convert the flattened index n (1‑based) into row r and column k
   of the lower triangular matrix. */
static void index_to_rc(idx_t n, idx_t *r, idx_t *c)
{
    idx_t low = 1, high = 1;
    /* find high such that high(high+1)/2 >= n */
    while (((unsigned __int128)high * (high + 1)) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (((unsigned __int128)mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    *r = low;
    /* start index of this row = (r-1)r/2 + 1 */
    idx_t prev = ((unsigned __int128)(low - 1) * low) / 2;
    *c = n - prev;              /* 1‑based column */
}

/* Compute the triangle entry T(r,k). */
static seq_t compute_T(idx_t r, idx_t k)
{
    seq_t bin = binom_uint64(r, k);
    seq_t fact = 1;
    for (idx_t i = k; i < r; ++i)   /* product i from k to r-1 */
        fact *= (seq_t)i;
    return bin * fact;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; a(0) is unused */
        return 0;
    idx_t r, k;
    index_to_rc(n, &r, &k);
    return compute_T(r, k);
}
