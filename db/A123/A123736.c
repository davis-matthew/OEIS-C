/*
 * A123736  Triangle T(n,k) = Sum_{j=0..k/2} binomial(n-j-1,k-2*j), read by
 *          rows.
 * https://oeis.org/A123736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   T(n,k) = sum_{j=0..⌊k/2⌋} binomial(n-j-1, k-2*j); a(n) is the
 *            nth term when the triangle T is read row‑by‑row
 * Range:     u8 n <= 115, u16 n <= 351, u32 n <= 1275, u64 n <= 4588,
 *            u128 n <= 9524, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binomial_calc(idx_t n, idx_t k)
{
    /* safe binomial computation that works for all supported seq_t types */
    if (k > n) return 0;
    if (k > n - k) k = n - k;

    /* For fixed‑size seq_t use 128‑bit intermediate to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        __uint128_t res = 1;
        for (idx_t i = 1; i <= k; ++i) {
            res = res * (n - k + i);
            res /= i;
        }
        return (seq_t)res;
    }
    /* For arbitrary‑precision seq_t use the natural seq_t arithmetic. */
    else {
        seq_t res = 1;
        for (idx_t i = 1; i <= k; ++i) {
            res = res * (seq_t)(n - k + i) / (seq_t)i;
        }
        return res;
    }
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1) >= n. */
    idx_t r = 1;
    while ((__uint128_t)r * (r + 1) < n) {
        ++r;
    }

    /* Column position (0‑based) in row r. */
    idx_t start_of_row = (r - 1) * r;
    idx_t k = n - start_of_row - 1;

    /* Compute T(r,k). */
    seq_t sum = 0;
    idx_t j_max = k / 2;
    for (idx_t j = 0; j <= j_max; ++j) {
        idx_t upper = r - j - 1;
        idx_t lower = k - 2 * j;
        if (lower <= upper) {
            sum += binomial_calc(upper, lower);
        }
    }
    return sum;
}
