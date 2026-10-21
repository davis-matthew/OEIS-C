/*
 * A096823  a(n) = p*(p+(2n-1))/2, where p = A096822(n) is the smallest primes
 *          of form 2^x-(2n-1).
 * https://oeis.org/A096823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = p * (p + (2n-1)) / 2, where p is the smallest prime of
 * the form 2^x - (2n-1).
 *
 * The computed value can be expressed as a(n) = p * 2^(x-1).
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Small helper: floor(log2(n)) for non‑zero unsigned integers. */
static inline idx_t floor_log2_uint64(idx_t n)
{
#if defined(__GNUC__) || defined(__clang__)
    /* GCC/Clang built‑in, defined for GCC and Clang compliant C/C++. */
    return n == 0 ? 0 : (sizeof(idx_t) * 8 - 1 - __builtin_clzll(n));
#else
    idx_t r = 0;
    while ((idx_t)1 << (r + 1) <= n) ++r;
    return r;
#endif
}

/* Small helper: 2^exp as seq_t. */
static inline seq_t pow2_seq(idx_t exp)
{
    return ((seq_t)1) << exp;
}

seq_t a(idx_t n)
{
    /* a(1) is the first term; behaviour for n < 1 is irrelevant. */
    if (n < 1u) {
        return (seq_t)0;
    }

    /* Target t = 2n-1.  We need p = 2^x - t to be prime. */
    idx_t target = (idx_t)2 * n - (idx_t)1;

    /* Start from the smallest exponent x such that 2^x > target. */
    idx_t x = floor_log2_uint64(target) + (idx_t)1;

    /* Search for the smallest x with prime p. */
    for (;;) {
        seq_t p = pow2_seq(x) - (seq_t)target;

        if (is_prime_big(p)) {
            /* a(n) = p * 2^(x-1). */
            seq_t a_val = p * pow2_seq(x - (idx_t)1);
            return a_val;
        }
        ++x;
    }
}
