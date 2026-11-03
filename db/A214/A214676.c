/*
 * A214676  A(n,k) is n represented in bijective base-k numeration; square
 *          array A(n,k), n>=1, k>=1, read by antidiagonals.
 * https://oeis.org/A214676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 65, u64 n <= 171, u128 n <= 171,
 *            big any n
 * Validated: standard, pass, 71 terms through n = 166 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A214676:  a(n) is the n-th entry of the square array
   A(i,j) of bijective base‑j representations of i, read by antidiagonals,
   with offset 1.  The function uses only integer arithmetic and the
   helper functions `digits_of` and `pow10` for digit counts and powers
   of ten. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Return the diagonal number d such that n ≤ d*(d+1)/2 */
static idx_t diagonal_idx(idx_t n)
{
    idx_t low = 1, high = 1;
    /* grow high until it satisfies the condition */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    /* binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset 1, but handle 0 gracefully */
        return 0;

    /* Determine coordinates (i,j) from antidiagonal index n */
    idx_t d = diagonal_idx(n);
    idx_t prev = (d - 1) * d / 2;
    idx_t offset = n - prev - 1;          /* 0‑based within diagonal */
    idx_t i = offset + 1;
    idx_t j = d - offset;

    /* Convert i to bijective base‑j representation and build result */
    seq_t digit_storage[64];
    int len = 0;
    idx_t temp = i;

    while (temp > 0) {
        idx_t r = temp % j;
        idx_t digit = (r == 0) ? j : r;
        temp = (r == 0) ? temp / j - 1 : temp / j;
        digit_storage[len++] = (seq_t)digit;
    }

    seq_t result = 0;
    for (int k = len - 1; k >= 0; --k) {
        idx_t dlen = (idx_t)num_digits_idx((uint64_t)digit_storage[k]);
        seq_t pow = pow10(dlen);
        result = result * pow + digit_storage[k];
    }
    return result;
}
