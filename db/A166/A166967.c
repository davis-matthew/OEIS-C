/*
 * A166967  Triangle read by rows, (Sierpinski's gasket, A047999) * A166966
 *          (diagonalized as a lower triangular matrix).
 * https://oeis.org/A166967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = S(i,j) * q[j] where (i,j) is the position of n in the
 *            triangle (row i, column j). S(i,j) = C(i,j) mod 2, which equals
 *            1 iff (j & (i-j)) == 0. The eigenvalues q[k] are defined by q[0]
 *            = 1, q[1] = 1, and for n >= 1: q[n] = q[n-1] + sum_{k=0}^{n-2}
 *            [C(n-1,k) mod 2] * q[k]
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   S_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/S_parity.h"

/* integer square root for unsigned __int128 (returns floor(sqrt(x))) */
static inline idx_t isqrt_uint128(unsigned __int128 x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1ULL << 63;           /* highest bit of a 64‑bit integer */
    while (bit) {
        idx_t trial = r | bit;
        unsigned __int128 sq = (unsigned __int128)trial * trial;
        if (sq <= x) {
            r = trial;
        }
        bit >>= 1;
    }
    return r;
}

/* triangular number offset: row i starts at i*(i+1)/2 */
static inline idx_t offset_row(idx_t i)
{
    return (idx_t)(((__int128)i * (i + 1)) / 2);
}

/* find row index i such that offset_row(i) <= n < offset_row(i+1) */
static inline idx_t row_index(idx_t n)
{
    /* compute i = floor((sqrt(8n+1)-1)/2) */
    unsigned __int128 prod = (unsigned __int128)8 * n + 1;
    idx_t s = isqrt_uint128(prod);
    idx_t i = (s - 1) / 2;

    /* Correct possible off‑by‑one caused by integer arithmetic. */
    while (( (__int128)(i + 1) * (i + 2) / 2 ) <= n) ++i;
    while (( (__int128)i * (i + 1) / 2 ) > n) --i;
    return i;
}

/* dynamic storage of the eigensequence q[k] */
static seq_t *q_cache   = NULL;   /* array of q values */
static size_t   q_cap   = 0;      /* allocated capacity */
static idx_t    q_len   = 0;      /* number of elements currently stored */

/* Ensure q_cache contains values up to index 'need' inclusive. */
static void ensure_q(idx_t need)
{
    /* grow capacity if required */
    if (need + 1 > q_cap) {
        size_t new_cap = q_cap ? q_cap * 2 : 4;
        while (new_cap <= (size_t)need + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(q_cache, new_cap * sizeof(seq_t));
        q_cache = tmp;
        q_cap   = new_cap;
    }

    /* initialize base values */
    if (q_len == 0) {
        q_cache[0] = (seq_t)1;
        q_len = 1;
    }

    /* compute new elements */
    while (q_len <= need) {
        idx_t i = q_len - 1;      /* i corresponds to the recurrence index */
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            if ((k & (i - k)) == 0) {
                sum += q_cache[k];
            }
        }
        q_cache[q_len] = q_cache[q_len - 1] + sum;
        ++q_len;
    }
}

/* required function for the test harness */
seq_t a(idx_t n)
{
    /* locate row and column of linear index n */
    idx_t i     = row_index(n);
    idx_t off   = offset_row(i);
    idx_t j     = n - off;               /* column within the i‑th row */

    /* compute q[j] if it does not exist yet */
    ensure_q(j);

    /* if S(i,j) == 0 then the product is zero */
    if (!S_parity(i, j)) {
        return (seq_t)0;
    }

    /* otherwise return eigenvalue q[j] */
    return q_cache[j];
}
