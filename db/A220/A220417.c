/*
 * A220417  Table T(n,k) = k^n - n^k, n, k > 0, read by descending
 *          antidiagonals.
 * https://oeis.org/A220417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = k^n - n^k where (n,k) are obtained by reading the table
 *            T(i,j) = j^i - i^j along descending antidiagonals (i+j constant,
 *            i increasing)
 * Range:     i8 n <= 30, i16 n <= 58, i32 n <= 141, i64 n <= 386,
 *            i128 n <= 1091, big any n
 * Helpers:   _floor_isqrt_u64, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sufficient utility headers */
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the sum S = i + j for which the cumulative count up to S-1
       is less than n and cumulative up to S is at least n. */
    idx_t s_val   = 1 + 8 * n;                 /* 1 + 8*n for sqrt */
    idx_t sqrt_val = _floor_isqrt_u64(s_val); /* floor(sqrt(1+8n)) */

    /* Ceil((1 + sqrt(1 + 8n))/2).  The formula with integer division
       already gives the ceil when we add 1 before dividing. */
    idx_t sum = 1 + sqrt_val;                 /* 1 + floor(sqrt) */
    idx_t S = (sum + 1) / 2;                  /* ceil division */

    /* Correct any under‑estimation caused by floor(sqrt) */
    if ((S - 1) * S / 2 < n)
        ++S;

    /* Compute the previous cumulative total */
    idx_t prev = (S - 2) * (S - 1) / 2;       /* pairs with i+j < S */

    /* offset inside the current antidiagonal (1 based) */
    idx_t offset = n - prev;                  /* 1 <= offset <= S-1 */

    /* Coordinates of the table entry: i = offset, j = S - i */
    idx_t i = offset;
    idx_t j = S - offset;

    /* Compute the two powers using the arbitrary‑precision routine */
    seq_t power1 = big_pow((seq_t)j, (seq_t)i);   /* j^i */
    seq_t power2 = big_pow((seq_t)i, (seq_t)j);   /* i^j */

    /* Return the difference as specified */
    return power1 - power2;
}
