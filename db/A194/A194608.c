/*
 * A194608  Smallest prime either of the form prime(n)*2^k - 1 or prime(n)*2^k
 *          + 1, k >= 0, or 0 if no such prime exists, where prime(n) denotes
 *          the n-th prime number.
 * https://oeis.org/A194608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime of the form prime(n) * 2^k ± 1 (k ≥ 0) or
 *            0 if no such prime exists
 * Range:     u8 n <= 13, u16 n <= 52, u32 n <= 52, u64 n <= 228,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t, get_prime, is_prime_big, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

/* Helper functions provided by the build environment */
#include "../utils/get_prime.h"        /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime_big.h"     /* bool is_prime_big(seq_t n); */
#include "../utils/mul_small.h"        /* seq_t mul_small(seq_t x, unsigned int m); */
#include "../utils/bitlen_seq_t.h"    /* unsigned bitlen_seq_t(seq_t x); */

seq_t a(idx_t n)
{
    if (n == 0)                /* safety for out‑of‑range calls */
        return 0;

    /* Obtain the n‑th prime (1‑based) */
    uint32_t p_u32 = get_prime(n);
    seq_t    p = (seq_t)p_u32;

    seq_t cur = p;             /* prime(n) * 2^k with k = 0 initially */

    for (;;)
    {
        /* Test the −1 candidate. */
        seq_t minus = cur - 1;
        if (minus >= 2 && is_prime_big(minus))
            return minus;

        /* Test the +1 candidate. */
        seq_t plus = cur + 1;
        if (is_prime_big(plus))
            return plus;

        /* Prepare for the next k: multiply by 2.
           Stop if the next doubling would overflow the underlying type. */
        unsigned bits = bitlen_seq_t(cur);
        if (bits >= sizeof(seq_t) * CHAR_BIT - 1)
            break;   /* further doubling would overflow          */

        cur = mul_small(cur, 2);
    }

    /* No prime found for any k within the representable range. */
    return 0;
}
