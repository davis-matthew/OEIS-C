/*
 * A131990  Consider the periodic sequence S(n) defined by: b(1)=2, b(i+1) =
 *          b(i) + n if i is prime, b(i+1) = gpf(b(i)) if i is not prime,
 *          where gpf means "greatest prime factor"; then a(n) is the length
 *          of the repeating period of the sequence.
 * https://oeis.org/A131990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the length of the repeating period of the sequence
   defined by b(1)=2 and,
   b(i+1) = b(i)+n  if  b(i) is prime,
   b(i+1) = gpf(b(i))  otherwise.
   a(n) is the size of the eventual cycle of this sequence.   */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/largest_prime_factor.h"

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

/* Step function of the recurrence. */
static inline seq_t step(seq_t cur, seq_t N)
{
    return isPrimeGeneric(cur) ? cur + N : (seq_t)largest_prime_factor((uint64_t)cur);
}

/* Public function required by the exerciser. */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* cast index to seq_t once */
    seq_t tort = step((seq_t)2, N);
    seq_t hare = step(step((seq_t)2, N), N);

    /* Find first intersection of tortoise and hare (Floyd). */
    while (tort != hare) {
        tort = step(tort, N);
        hare = step(step(hare, N), N);
    }

    /* Compute the length λ of the cycle. */
    seq_t lambda = (seq_t)1;
    hare = step(tort, N);
    while (tort != hare) {
        hare = step(hare, N);
        ++lambda;
    }
    return lambda;
}
