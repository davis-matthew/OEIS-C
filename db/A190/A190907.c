/*
 * A190907  Triangle read by rows: T(n,k) = binomial(n+k, n-k) k! /
 *          (floor(k/2)! * floor((k+2)/2)!).
 * https://oeis.org/A190907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 42, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _fact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A190907: a(n) = T(r,k) with
 *   r = floor( (-1 + sqrt(1+8n))/2 ),   k = n - r(r+1)/2
 *   T(r,k) = binomial(r+k, r-k) * k! /
 *            ( floor(k/2)! * floor((k+2)/2)! )
 * Note: rows are read in order: the sequence is the triangle flattened
 * by rows.  This implementation computes the term using only integer
 * arithmetic and does not rely on any external helper functions.
 */

#include <stddef.h>
#include "../utils/_fact.h"

/* ------------------------------------------------------------------ */

/* Helper: multiplicative binomial coefficient ----------------------- */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;          /* exact division at each step */
    }
    return res;
}

/* Helper: find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 --------- */
static idx_t _find_row(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ------------------------------------------------------------------ */
/* Sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t r = _find_row(n);
    seq_t tri_r = (seq_t)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tri_r;

    seq_t bin = _binom(r + k, r - k);
    seq_t fk  = _fact(k);
    seq_t f1  = _fact(k >> 1);                  /* floor(k/2) */
    seq_t f2  = _fact((k + 2) >> 1);            /* floor((k+2)/2) */

    seq_t result = bin * fk;
    result /= f1;
    result /= f2;
    return result;
}
