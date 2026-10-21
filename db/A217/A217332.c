/*
 * A217332  Orders n of Hadamard cyclic difference sets.
 * https://oeis.org/A217332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 255, u32 n <= 255, u64 n <= 255,
 *            u128 n <= 255, big any n
 * Helpers:   _is_mersenne
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Orders n of Hadamard cyclic difference sets.
   Computed by generating numbers congruent to 3 mod 4 and testing
   whether each is prime, a Mersenne number (n+1 power of two),
   or a product of twin primes. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/_is_mersenne.h"

static bool _is_prime(idx_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return false;
    return true;
}

static uint64_t _smallest_factor(idx_t n)
{
    if ((n & 1) == 0) return 2;
    for (uint64_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n is prime */
}

static bool _is_twin_product(idx_t n)
{
    uint64_t sp = _smallest_factor(n);
    if (sp >= n) return false;           /* n is prime */
    idx_t other = n / sp;
    if (other != sp + 2) return false;   /* not a twin pair */
    return _is_prime(other);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0, but guard */
    idx_t count = 0;
    for (idx_t cur = 3; ; cur += 4) {     /* iterate only n ≡ 3 (mod 4) */
        if (_is_prime(cur) || _is_mersenne(cur) || _is_twin_product(cur)) {
            ++count;
            if (count == n) return (seq_t)cur;
        }
    }
}
