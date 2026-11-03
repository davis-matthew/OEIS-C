/*
 * A076302  Triangle T(n,k) = number of k-smooth divisors of n, read by rows.
 * https://oeis.org/A076302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) is the entry of the triangle T(r,k) where
 *   r = smallest integer with r(r+1)/2 ≥ n         (row index)
 *   k = n - (r-1)r/2                               (column index)
 * T(r,k) = number of k‑smooth divisors of r
 *         = ∏_{p^e‖r, p≤k} (e+1)
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute row and column from the linear index n */
static void row_col(idx_t n, idx_t *r, idx_t *k)
{
    idx_t low = 1, high = 1;

    /* Find an upper bound for r by doubling */
    while ((__int128)high * (high + 1) / 2 < n)
        high <<= 1;

    /* Binary search for the smallest r with r(r+1)/2 ≥ n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    *r = low;

    idx_t first = (low - 1) * low / 2 + 1;
    *k = n - first + 1;
}

/* Count k‑smooth divisors of n (n ≥ 1) */
static seq_t smooth_divisor_count(idx_t n, idx_t k)
{
    seq_t result = 1;
    idx_t temp = n;

    /* Handle factor 2 separately for efficiency */
    if (temp % 2 == 0) {
        if (2 <= k) {
            idx_t exp = 0;
            while (temp % 2 == 0) {
                temp /= 2;
                exp++;
            }
            result *= (exp + 1);
        } else {
            while (temp % 2 == 0) temp /= 2;
        }
    }

    /* Trial division by odd primes */
    for (idx_t p = 3; (uint64_t)p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if (p <= k) {
                idx_t exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    exp++;
                }
                result *= (exp + 1);
            } else {
                while (temp % p == 0) temp /= p;
            }
        }
    }

    /* Remaining prime factor (if any) */
    if (temp > 1) {
        if (temp <= k) result *= 2; /* exponent 1 → (1+1) */
    }

    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t r, k;
    row_col(n, &r, &k);
    return smooth_divisor_count(r, k);
}
