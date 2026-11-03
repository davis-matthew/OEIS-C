/*
 * A125557  Primes in the sequence a(n)=n^2+3/2-1/2*(-1)^n.
 * https://oeis.org/A125557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 + 1 when n is even, n^2 + 2 when n is odd, but only
 *            the * primes of that form are included in the sequence. Since
 *            not every such * value is prime, the algorithm searches
 *            sequentially for the nth prime of * the form n^2 + 1 (even) or
 *            n^2 + 2 (odd)
 * Range:     u8 n <= 9, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; ; ++m)
    {
        seq_t val = (seq_t)m * (seq_t)m;
        val += (m & 1ULL) ? 2 : 1;   /* add 2 for odd m, 1 for even m */
        if (isPrimeGeneric(val))
        {
            ++count;
            if (count == n)
                return val;
        }
    }
}
