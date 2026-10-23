/*
 * A159353  a(n) = the smallest positive integer such that a(n)*(2^n - 2) is a
 *          multiple of n.
 * https://oeis.org/A159353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / gcd(n, 2^n – 2)
 * Range:     u8 n <= 274, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_idx_t, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the smallest positive integer such that a(n)*(2^n-2) is a multiple of n. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/gcd_idx_t.h"

seq_t a(idx_t n)
{
    /* offset starts at 1 */
    if (n == 0) {
        /* undefined for 0, return 0 to avoid UB */
        return (seq_t)0;
    }
    if (n == 1) {
        return (seq_t)1;
    }

    /* Compute 2^n modulo n */
    seq_t mod_val = mod_pow((seq_t)2, n, (seq_t)n);
    idx_t r = (idx_t)mod_val;                /* remainder in [0, n-1] */
    idx_t diff = (r >= 2) ? (r - 2) : (r + n - 2); /* (2^n - 2) mod n */

    /* Compute gcd(n, diff) using 64-bit arithmetic to avoid seq_t overflow */
    idx_t g = gcd_idx_t(n, diff);

    /* a(n) = n / g */
    seq_t result = (seq_t)(n / g);
    return result;
}
