/*
 * A174428  Triangle read by rows: R(n,k) = sigma(n) mod k, where sigma(.) is
 *          the sum of divisors.
 * https://oeis.org/A174428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) reads the triangle R(n,k)=σ(n) mod k. For the linear index
 *            n (offset 1), find the unique r such that r*(r+1)/2 ≥ n and
 *            r*(r-1)/2 < n. Let k = n - r*(r-1)/2. Then a(n) = σ(r) mod k
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t sigma_u64(idx_t n)
{
    if (n == 0) return 0;
    uint64_t sum = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t term = 1;          /* part for this prime factor */
            uint64_t pow_term = 1;      /* current power of p */
            while (temp % p == 0) {
                temp /= p;
                pow_term *= p;
                term += pow_term;       /* (p^(e+1)-1)/(p-1) */
            }
            sum *= term;
        }
    }
    if (temp > 1) {                      /* remaining prime factor */
        sum *= (1 + temp);
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for out of range */

    /* find row r: smallest r with r*(r+1)/2 >= n */
    idx_t r = 1;
    idx_t tri = 1;                       /* r*(r+1)/2 */
    while (tri < n) {
        ++r;
        tri = r * (r + 1) / 2;
    }

    /* previous triangular number */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;              /* column index (1‑based) */

    uint64_t sigma = sigma_u64(r);
    return (seq_t)(sigma % k);
}
