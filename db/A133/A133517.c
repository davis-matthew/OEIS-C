/*
 * A133517  Smallest k such that p(n)^3 - k is prime where p(n) is the n-th
 *          prime.
 * https://oeis.org/A133517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 274, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = smallest k such that p(n)^3 - k is prime
 * where p(n) is the nth prime (1‑indexed).  For n=1, k must be odd,
 * otherwise k must be even.  The function uses the provided
 * utilities get_prime, is_prime_small and is_prime_big.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"     /* get_prime(idx_t k) -> uint32_t */
#include "../utils/is_prime_small.h"/* is_prime_small(seq_t n) -> bool */
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t n)   -> bool */

static inline bool seq_is_prime(seq_t x)
{
    /* Select the appropriate primality routine depending on the
     * size of seq_t at compile time.  For 64‑bit (or smaller) types
     * use the fast trial version; otherwise fall back to the generic
     * arbitrary‑precision test. */
    if (sizeof(seq_t) <= sizeof(uint64_t))
        return is_prime_small(x);
    else
        return is_prime_big(x);
}

seq_t a(idx_t n)
{
    /* Get the nth prime (1‑indexed) as a 32‑bit integer and cast it
     * to seq_t.  The prime table utilities guarantee that the
     * requested prime fits into uint32_t. */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Compute the cube p^3.  Intermediate values are all of type
     * seq_t to avoid overflow in integral types smaller than 64 bits. */
    seq_t base = p * p * p;

    /* Special case for n = 1: base is even, so k must be odd.
     * For n > 1: base is odd, so k must be even. */
    seq_t k = (n == 1) ? (seq_t)1 : (seq_t)2;

    for (;;) {
        seq_t candidate = base - k;
        if (seq_is_prime(candidate))
            return k;
        k += (n == 1) ? (seq_t)2 : (seq_t)2;
    }
}
