/*
 * A185342  Triangle of successive recurrences in columns of A117317(n).
 * https://oeis.org/A185342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (-1)^(k-1) * 2^k * binom(r, k) where r is the 1‑based row
 *            index such that r*(r-1)/2 <= n < r*(r+1)/2 and k is the 1‑based
 *            column index given by k = n - r*(r-1)/2 + 1
 * Range:     i8 n <= 16, i16 n <= 60, i32 n <= 241, i64 n <= 885,
 *            i128 n <= 3452, big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow_uint.h"

/* Compute row index r (1‑based) for given zero‑based linear index n. */
static inline idx_t
row_from_index(idx_t n)
{
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high - 1) / 2 <= n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * (mid - 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    /* Determine row r and column k from the linear index n. */
    idx_t r = row_from_index(n);
    idx_t start = r * (r - 1) / 2;
    idx_t k = n - start + 1;     /* 1‑based column index */

    /* Compute binom(r, k). */
    seq_t bin = binom(r, k);

    /* Compute 2^k. */
    seq_t pow2 = pow_uint(2, k);

    /* Multiply and apply sign. The sign is negative when k is even. */
    seq_t res = bin * pow2;
    if ((k & 1) == 0)   /* even column -> negative sign */
        res = -res;

    return res;
}
