/*
 * A061028  a(n) = n^p - (n-1)^p, where p is the n-th prime.
 * https://oeis.org/A061028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^p - (n-1)^p, where p = the n‑th prime
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#include "../utils/get_prime.h"
#include "../utils/seq_mul.h"

/* Exponentiation by repeated squaring.
   Base: seq_t, Exponent: uint32_t. */
static inline seq_t pow_seq_uint(seq_t base, uint32_t exp)
{
    seq_t result = 1;
    seq_t cur    = base;
    while (exp)
    {
        if (exp & 1U)
            result = seq_mul(result, cur);
        exp >>= 1U;
        if (exp)
            cur = seq_mul(cur, cur);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* Offset is 1; value for n=0 is unused */

    /* Obtain the n‑th prime. */
    uint32_t pk = get_prime(n);    /* kth prime, 1‑based index */

    /* Powers. */
    seq_t pow_n    = pow_seq_uint((seq_t)n,     pk);
    seq_t pow_nm1  = pow_seq_uint((seq_t)(n-1), pk);

    /* Difference. */
    return pow_n - pow_nm1;
}
