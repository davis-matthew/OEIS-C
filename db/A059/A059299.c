/*
 * A059299  Triangle of idempotent numbers (version 3), T(n, k) = binomial(n,
 *          k) * (n - k)^k.
 * https://oeis.org/A059299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened sequence of the triangle * T(r,k) =
 *            binomial(r,k) * (r-k)^k (0^0 is defined as 1). * a(n) is
 *            obtained by locating the unique row r with * r*(r+1)/2 <= n <
 *            (r+1)*(r+2)/2 and setting k = n - r*(r+1)/2
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 112, u64 n <= 289,
 *            u128 n <= 803, big any n
 * Helpers:   binom_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/binom_seq_t.h"

/* integer power: base^exp, exponentiation by squaring.
 * Works for base=0 and exp=0 (returns 1). */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            result = result * base;
        base = base * base;
        exp >>= 1U;
    }
    return result;
}

/* main sequence */
seq_t a(idx_t n)
{
    /* Determine row r:  r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t r;
    {
        /* Use double for a quick estimate; adjust afterwards. */
        long double d = sqrtl((long double)(8.0L * (long double)n + 1.0L));
        r = (idx_t)((d - 1.0L) / 2.0L);
        /* Correct possible rounding errors */
        while (((__uint128_t)(r + 1) * (r + 2)) / 2 <= n) ++r;
        while (((__uint128_t)r * (r + 1)) / 2 > n) --r;
    }

    /* k = position in row r */
    idx_t start_of_row = (idx_t)(((idx_t)r * (r + 1)) / 2);
    idx_t k = n - start_of_row;

    /* Compute T(r,k) = C(r,k) * (r-k)^k */
    seq_t bin = binom_seq_t(r, k);
    seq_t pow = pow_seq_t((seq_t)(r - k), k);
    return bin * pow;
}
