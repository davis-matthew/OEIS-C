/*
 * A174875  Triangle read by rows: T(n,k) = sigma(n) mod tau(k), where
 *          sigma(.) is the sum of divisors and tau(.) is the number of
 *          divisors.
 * https://oeis.org/A174875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = sigma(r) mod tau(k) where r = smallest integer with
 *            r*(r+1)/2 >= n (row number) and k = n - r*(r-1)/2 (column
 *            number)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/triangular.h"

/* Return r*(r+1)/2 using 128‑bit intermediate to avoid overflow. */

/* Sum of divisors function sigma(m) for unsigned 64‑bit m. */
static idx_t sigma_of(idx_t m)
{
    idx_t result = 1;
    idx_t p = 2;
    idx_t temp = m;

    while ((idx_t)p * p <= temp) {
        if (temp % p == 0) {
            idx_t sum_p = 1;
            idx_t power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= p;
                sum_p += power;
            }
            result *= sum_p;
        }
        ++p;
    }
    if (temp > 1) {
        result *= (1 + temp);
    }
    return result;
}

/* Number of divisors function tau(m) for unsigned 64‑bit m. */
static idx_t tau_of(idx_t m)
{
    idx_t result = 1;
    idx_t temp = m;
    idx_t p = 2;

    while ((idx_t)p * p <= temp) {
        if (temp % p == 0) {
            idx_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            result *= (cnt + 1);
        }
        ++p;
    }
    if (temp > 1) {
        result *= 2;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find row r and column k for linear index n (1‑based). */
    idx_t low = 1, high = 1;
    while (triangular(high) < n)
        high <<= 1;  /* exponential search to bound the row number */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t r = low;                    /* row number */
    idx_t prev = triangular(r - 1);   /* sum of elements before row r */
    idx_t k = n - prev;                /* column number (1‑based within row) */

    /* Compute sigma(r) and tau(k) using 64‑bit arithmetic. */
    idx_t sigma_val = sigma_of(r);
    idx_t tau_val   = tau_of(k);

    /* Return the value modulo tau(k), cast to seq_t. */
    return (seq_t)(sigma_val % tau_val);
}
