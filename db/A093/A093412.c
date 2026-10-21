/*
 * A093412  Triangle read by rows: a(n, k) is the numerator of (n + (n-1) +
 *          ... + (n-k+1))/(1 + 2 + ... + k), 0 < k <= n.
 * https://oeis.org/A093412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, frac
 * Formula:   a(n) = (2n+1‑k)/gcd(2n+1‑k, k+1) where k is the column index in
 *            the triangle obtained from the flattened index n
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   cum_ge
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/cum_ge.h"

/* Euclidean algorithm for seq_t (works for any integer type, including
   arbitrary‑precision implementations that support % and ==). */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Find the row r (1‑based) such that the flattened index n is
   contained in that row.  Uses exponential search followed by binary
   search. */
static idx_t row_of(idx_t n)
{
    idx_t low = 1, high = 1;
    while (!cum_ge(high, n))
        high <<= 1;             /* exponential growth */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cum_ge(mid, n))
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Return the nth term of the flattened triangle A093412. */
seq_t a(idx_t n)
{
    /* Row containing the index n. */
    idx_t r = row_of(n);

    /* Number of terms before row r: (r-1)*r/2. */
    idx_t prev = (r - 1) * r / 2;

    /* Column index k within the row (1‑based). */
    idx_t k = n - prev;          /* 1 ≤ k ≤ r */

    /* Calculate the numerator and denominator of the fraction. */
    seq_t num = (seq_t)2 * (seq_t)r + (seq_t)1 - (seq_t)k;   /* 2r + 1 - k */
    seq_t den = (seq_t)k + (seq_t)1;                         /* k + 1 */

    /* Reduce by their greatest common divisor. */
    seq_t g = gcd_seq_t(num, den);
    return num / g;
}
