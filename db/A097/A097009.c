/*
 * A097009  a(n) = gcd(prime(2*n)-1, prime(n)-1).
 * https://oeis.org/A097009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(prime(2*n)-1, prime(n)-1)
 * Range:     u8 n <= 210, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b)
    {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* prime indices are 1‑based */
    uint32_t p_n    = get_prime(n);      /* prime(n)   */
    uint32_t p_2n   = get_prime(2 * n);  /* prime(2*n) */

    idx_t v1 = (idx_t)p_n  - (idx_t)1;
    idx_t v2 = (idx_t)p_2n - (idx_t)1;

    idx_t g = gcd_idx(v1, v2);

    return (seq_t)g;
}
