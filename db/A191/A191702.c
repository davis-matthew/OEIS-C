/*
 * A191702  Dispersion of A008587 (5,10,15,20,25,30,...), by antidiagonals.
 * https://oeis.org/A191702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (i-1+floor((i+3)/4))*5^(j-1), where (i,j) are found by
 *            reading the infinite table T(i,j) along its anti‑diagonals as
 *            described in OEIS A191702
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 105, u64 n <= 406,
 *            u128 n <= 1596, big any n
 * Helpers:   pow5_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow5_idx.h"

/* Multiply arithmetic in seq_t is overloaded by the build system.
   We implement a power‑of‑five helper that works for any seq_t. */

/* Find the anti‑diagonal number k such that
   k(k+1)/2 >= n, using binary search.
   Works for any n >= 1. */
static idx_t diagonal_of(idx_t n) {
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        /* Use 128‑bit to avoid overflow in the product. */
        __int128 prod = (__int128)mid * (mid + 1) / 2;
        if (prod >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n == 0)  /* offset is 1; not used but harmless */
        return 0;

    /* Locate the diagonal k containing the nth term. */
    idx_t k = diagonal_of(n);

    /* Number of terms before this diagonal. */
    __int128 prev128 = (__int128)(k - 1) * k / 2;
    idx_t prev = (idx_t)prev128;   /* fits in idx_t because prev <= n-1 */

    idx_t idx = n - prev;          /* 1‑based index within diagonal */

    idx_t i = idx;                 /* row number */
    idx_t j = k - idx + 1;         /* column number */

    /* Compute B(i) = (i-1) + floor((i+3)/4). */
    seq_t B = (seq_t)(i - 1) + (seq_t)((i + 3) / 4);

    /* Compute 5^(j-1). */
    seq_t pow5 = pow5_idx(j - 1);

    return B * pow5;
}
