/*
 * A185506  Accumulation array, T, of the natural number array A000027, by
 *          antidiagonals.
 * https://oeis.org/A185506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 585, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A185506 (Accumulation array T of A000027 by antidiagonals).
 * Formula: T(n,k) = k*n*(2*n^2 + 3*(k+1)*n + 2*k^2 - 3*k + 5)/12.
 * The sequence a(n) enumerates T in anti‑diagonal order (n+k constant,
 * ascending n).  a(1)=1, a(2)=3, a(3)=4, …
 */

#include <stdint.h>
#include <stddef.h>

/* Computes the accumulation array element T(n,k). */
static seq_t T_value(idx_t n, idx_t k)
{
    /* All operations are performed in seq_t to preserve precision. */
    seq_t nn = (seq_t)n;
    seq_t kk = (seq_t)k;
    seq_t term = (seq_t)2 * nn * nn
               + (seq_t)3 * (kk + (seq_t)1) * nn
               + (seq_t)2 * kk * kk
               - (seq_t)3 * kk
               + (seq_t)5;
    seq_t num = kk * nn * term;   /* numerator of the formula   */
    return num / (seq_t)12;       /* exact division (denominator 12) */
}

/* Finds the minimal S such that S*(S-1)/2 ≥ idx.  Uses only integer arithmetic. */
static idx_t diagonal_sum(idx_t idx)
{
    idx_t low = 1, high = 1;
    /* Increase upper bound until high*(high-1)/2 ≥ idx. */
    while ( ((unsigned __int128)high * (high - 1)) / 2 < idx )
        high <<= 1;

    /* Binary search for smallest S satisfying S*(S-1)/2 ≥ idx. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( ((unsigned __int128)mid * (mid - 1)) / 2 >= idx )
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Public function implementing the sequence. */
seq_t a(idx_t n)
{
    /* n ≥ 1 – negative or zero indices are not used. */
    idx_t idx = n;             /* 1‑based position in the sequence */

    /* Determine the diagonal (sum of indices) in which this element lies. */
    idx_t S = diagonal_sum(idx);                /* S = n + k */
    idx_t prev = ((idx_t)((idx_t)(S - 2) * (S - 1)) ) / 2;   /* terms before this diagonal */
    idx_t offset = idx - prev - 1;            /* 0‑based position within the diagonal */
    idx_t row = offset + 1;                   /* n (row) */
    idx_t col = S - row;                      /* k (column) */

    return T_value(row, col);
}
