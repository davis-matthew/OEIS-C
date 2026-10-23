/*
 * A132046  Triangle read by rows: T(n,0) = T(n,n) = 1, and T(n,k) =
 *          2*binomial(n,k) for 1 <= k <= n - 1.
 * https://oeis.org/A132046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A132046: Triangle read by rows  
 * 
 *  T(n,0)=T(n,n)=1,  T(n,k)=2*C(n,k)  for 1<=k<=n-1
 *  a(n) is the linear index (row‑major order) of that triangle.
 *
 *  a(n) = 1                     if k==0 or k==r                (edge)
 *  a(n) = 2*binomial(r,k)      otherwise 
 *  where r = floor((sqrt(8*n+1)-1)/2)   // row containing index n
 *        k = n - r*(r+1)/2             // column within that row
 *
 *  The function below computes a(n) using only integer arithmetic,
 *  suitable for any build where seq_t and idx_t are unsigned integer 
 *  types (including arbitrary‑precision C++ class).
 */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit values.  Returns floor(sqrt(x)). */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = 1ULL << 62;          /* highest power of 4 <= x */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Multiplicative binomial coefficient.  Works for any seq_t. */
static seq_t binom_n_k(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (n - k + i)) / i;   /* exact division */
    }
    return res;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* determine the row r that contains index n */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;

    /* first index of that row */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    if (k == 0 || k == r)                /* edge elements */
        return (seq_t)1;

    seq_t val = binom_n_k(r, k);          /* C(r,k) */
    return val << 1;                      /* multiply by 2 */
}
