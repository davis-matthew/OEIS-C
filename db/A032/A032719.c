/*
 * A032719  Numbers k such that k prefixed by '2' and followed by '7' is
 *          prime.
 * https://oeis.org/A032719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k (k ≥ 1) such that the * concatenated
 *            number 2∘k∘7 is prime
 * Range:     u8 n <= 1, u16 n <= 277, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 975 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the helper isPrimeGeneric which is defined in
 * "../utils/isPrimeGeneric.h". */
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

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, guard against accidental 0 */
        return 0;

    idx_t  k          = 1;      /* current candidate k       */
    idx_t  next_mul10 = 10;     /* next threshold where digit count increases */
    seq_t  pow10d1    = 100;    /* 10^(digits(k)+1), starts with 10^2 = 100 */

    /* Count of primes found so far. */
    idx_t  count = 0;

    while (count < n) {
        /* Update digit count when k reaches 10, 100, 1000, ... */
        if (k == next_mul10) {
            next_mul10 *= 10;     /* next threshold          */
            pow10d1  *= 10;      /* increase 10^(d+1) factor */
        }

        /* Construct N = 2*10^(d+1) + k*10 + 7. */
        seq_t N = 2 * pow10d1 + (seq_t)k * 10 + 7;

        if (isPrimeGeneric(N)) {
            ++count;              /* found a new term of the sequence */
            if (count == n)
                return k;           /* return the n-th term */
        }

        ++k; /* consider next candidate k */
    }

    /* The loop always returns; this line is never reached but
     * follows the compiler's requirement that the function returns
     * a value of type seq_t. */
    return 0;
}
