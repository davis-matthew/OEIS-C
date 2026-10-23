/*
 * A131060  3*A007318 - 2*A000012 as infinite lower triangular matrices.
 * https://oeis.org/A131060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 160, u32 n <= 609, u64 n <= 2241,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the n‑th entry of the lower‑triangular array
    T(r,k) = 3*binomial(r,k) – 2 , with r = floor( (sqrt(8*n+1)-1)/2 )
    and k = n – r*(r+1)/2 .
    The array is read row‑wise, starting with row 0.  */
#include <stdbool.h>
#include <stdint.h>

/* ----- local binomial routine --------------------------------------------- */
/* Compute binomial(n,k) exactly using 128‑bit intermediates when the
   result fits in a 64‑bit integer; otherwise use the seq_t arithmetic. */
static seq_t binomial_calc(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;

    /* use symmetry */
    if (k > n - k) k = n - k;
    if (k == 0 || k == n) return (seq_t)1;

    seq_t res = (seq_t)1;

    /* If seq_t is 64‑bit or smaller, protect intermediate products
       with 128‑bit arithmetic. */
    if (sizeof(seq_t) <= 8) {
        for (idx_t i = 1; i <= k; ++i) {
            /* res = res * (n-k+i) / i  with 128‑bit safety */
            __uint128_t prod = (__uint128_t)res * (n - k + i);
            prod /= i;
            res = (seq_t)prod;
        }
    } else {
        /* seq_t is larger (e.g. 128‑bit or arbitrary‑precision) */
        for (idx_t i = 1; i <= k; ++i) {
            res = res * (seq_t)(n - k + i) / (seq_t)i;
        }
    }
    return res;
}

/* ----- main sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th element. The first element
       in row r is at linear index offset = r*(r+1)/2. */
    idx_t low = 0;
    idx_t high = n + 1;                    /* search space [0, n] */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t prod = (__uint128_t)mid * (mid + 1) / 2;
        if (prod <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;                     /* row index */

    /* Column index within the row */
    __uint128_t offset128 = (__uint128_t)r * (r + 1) / 2;
    idx_t offset = (idx_t)offset128;
    idx_t k = n - offset;

    /* Compute the triangle entry */
    seq_t bin = binomial_calc(r, k);        /* 3*binom(r,k) - 2 */
    seq_t result = bin * (seq_t)3 - (seq_t)2;
    return result;
}
