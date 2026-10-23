/*
 * A137370  Triangle: signed version of A055134.
 * https://oeis.org/A137370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, less
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 28, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (-1)^(r+k) * (r-2)^(r-k) * binomial(r-1, k-1), 
 * where r is the row number (starting at 1) of the triangle 
 * and k is the column index (1‑based) in that row. 
 * The linear index n is row‑major: n = 1·2/2 + 1·2/2 + … + (r-1)r/2 + k.
 * This is the signed version of OEIS A055134. 
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Fast integer power for seq_t base raised to a non‑negative idx_t exponent. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Return the smallest r such that r*(r+1)/2 >= n. */
static idx_t triangular_row(idx_t n)
{
    idx_t low = 1, high = 1;
    /* grow high until the triangular number exceeds n */
    while ((unsigned __int128)high * (high + 1) / 2 < n) high <<= 1;
    /* binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Handle the first index explicitly to avoid any off‑by‑one bugs. */
    if (n == 1) return 1;

    /* Determine row r and column k for the given linear index n. */
    idx_t r = triangular_row(n);
    idx_t prev = (r - 1) * r / 2;  /* number of terms before row r */
    idx_t k   = n - prev;          /* 1 <= k <= r */

    /* Sign factor: (-1)^(r+k). */
    seq_t sign = ((r + k) & 1) ? -1 : 1;

    /* Power factor: (r-2)^(r-k).  Handle 0^0 as 1 automatically. */
    seq_t base  = (seq_t)(r - 2);
    idx_t exp   = r - k;
    seq_t power = pow_seq(base, exp);

    /* Binomial coefficient C(r-1, k-1). */
    seq_t bin = binomial(r - 1, k - 1);

    /* Combine everything. */
    seq_t result = sign * power;
    result *= bin;
    return result;
}
