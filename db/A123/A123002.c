/*
 * A123002  Triangle read by rows: T(n, k) = 2^(n-1)*(2*k - 1) - 2^(k-1)*(2*n
 *          - 1).
 * https://oeis.org/A123002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(r,c) = 2^(r‑1)*(2*c‑1) – 2^(c‑1)*(2*r‑1), where r is
 *            the smallest integer with r(r+1)/2 ≥ n and c = n – r(r‑1)/2
 * Range:     i8 n <= 18, i16 n <= 82, i32 n <= 367, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* the sequence is defined for n ≥ 1 (offset 1) */
    if (n == 0) return 0;

    /* find the row r such that r(r+1)/2 ≥ n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        idx_t prefix = (idx_t)(prod / 2);
        if (prefix >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* compute column c */
    unsigned __int128 prod_prev = (unsigned __int128)(r - 1) * r;
    idx_t prev_count = (idx_t)(prod_prev / 2);
    idx_t c = n - prev_count;          /* 1‑based column index */

    /* compute the term using the formula */
    seq_t pow_r = pow2(r - 1);          /* 2^(r-1) */
    seq_t pow_c = pow2(c - 1);          /* 2^(c-1) */

    seq_t val1 = pow_r * ((seq_t)(2 * c - 1));
    seq_t val2 = pow_c * ((seq_t)(2 * r - 1));

    return val1 - val2;
}
