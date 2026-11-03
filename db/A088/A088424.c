/*
 * A088424  Number of primes in arithmetic progression starting with 13 and
 *          with d=2n.
 * https://oeis.org/A088424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primes in the arithmetic progression 13 + 2n·k
 *            (k = 0, 1, 2, …) until the first non‑prime term is encountered
 * Range:     u8 n <= 44, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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
    if (n < 1)
        return 0;                       /* offset starts at 1 */

    seq_t count = 0;                       /* number of primes found     */
    seq_t d     = (seq_t)2 * (seq_t)n;     /* common difference d = 2n   */
    seq_t term  = 13;                       /* first term of the progression */

    while (isPrimeGeneric(term)) {          /* keep counting primes      */
        ++count;
        term += d;                          /* next term in progression  */
    }
    return count;
}
