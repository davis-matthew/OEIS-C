/*
 * A181552  T(n,k) = gcd(n,k) A181549(k), triangle read by rows.
 * https://oeis.org/A181552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = gcd(r,k) * A181549(k) where r and k satisfy n = (r-1)*r/2
 *            + k, 1 ≤ k ≤ r and A181549(k) is multiplicative with A(p^e) = 1
 *            (e = 0) A(p) = p + 1 (e = 1) A(p^e) = p^e + p^{e-1} - p^{e-2} (e
 *            ≥ 2)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   compute_A181549
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_A181549.h"

/* Euclidean algorithm for seq_t */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Integer exponentiation: base^exp (exp fits in idx_t) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* invalid below offset */

    /* Find r such that r(r+1)/2 ≥ n  and (r-1)r/2 < n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Column index k (1‑based) */
    idx_t k = n - (r - 1) * r / 2;

    seq_t gcd_val = gcd_seq((seq_t)r, (seq_t)k);
    seq_t a181549_val = compute_A181549(k);

    /* Final value */
    return gcd_val * a181549_val;
}
