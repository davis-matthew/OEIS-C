/*
 * A127465  Triangle read by rows: T(n,k) = k*phi(n/k) if k|n, T(n,k)=0
 *          otherwise.
 * https://oeis.org/A127465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = k*phi(r/k) where r is the least integer with r*(r+1)/2 ≥
 *            n, k = n - (r-1)*r/2 (the column in row r), and phi(m) is
 *            Euler's totient function. a(1) = 1, a(2) = 1, a(3) = 2, a(4) =
 *            2, a(5) = 0, etc
 * Range:     u8 n <= 10011, u16 n <= 10011, u32 n <= 10011, u64 n <= 10011,
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 94 terms through n = 9525 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* --- totient for an unsigned integer n (idx_t) --------------------------- */
static seq_t phi_idx(idx_t n)
{
    seq_t result = (seq_t)n;
    seq_t temp   = (seq_t)n;
    idx_t i = 2;

    /* trial division to find distinct prime factors */
    while ((seq_t)i * (seq_t)i <= temp) {
        if (temp % i == 0) {
            result -= result / (seq_t)i;
            while (temp % i == 0)
                temp /= i;
        }
        ++i;
    }

    /* if any prime factor > sqrt(n) remains */
    if (temp > 1)
        result -= result / temp;

    return result;
}

/* --- sequence generator ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1 */

    /* find row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* column index k (1‑based) in row r */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;

    /* entry is zero unless k divides r */
    if (r % k != 0)
        return 0;

    /* compute k * phi(r/k) */
    idx_t m = r / k;
    seq_t phi_val = phi_idx(m);
    return (seq_t)k * phi_val;
}
