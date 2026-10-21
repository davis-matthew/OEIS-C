/*
 * A050229  Numbers k such that for any x in 1..k-1 there exists a y in 0..k-2
 *          such that x^2 == 2^y (mod k).
 * https://oeis.org/A050229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   factor_u64, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A050229.
   a(n) = 1, 2, or a prime p > 2 for which 2 is a primitive root modulo p.
   The algorithm simply tests consecutive odd numbers for primality and
   for the primitive‑root condition until the n‑th term is found.
   It requires only a handful of helper functions from the utils library. */

#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */
#include "../utils/factor_u64.h" /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

/* Test whether 2 is a primitive root modulo a prime p (p > 2). */
static bool is_primitive_root2(uint64_t p)
{
    if (p <= 2) return true;             /* 2 is trivially a primitive root mod 2 */
    uint64_t p1 = p - 1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(p1, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = primes[i];
        if (mod_pow((seq_t)2, p1 / q, (seq_t)p) == (seq_t)1)
            return false;
    }
    return true;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    /* Handle 1..2 directly. */
    if (n <= 2)
        return (seq_t)n;

    /* Number of qualifying primes required after the first two terms. */
    uint64_t need = n - 2;
    uint64_t found = 0;

    /* Start scanning odd numbers > 2. */
    uint64_t candidate = 3;
    while (found < need) {
        if (is_prime(candidate)) {
            if (is_primitive_root2(candidate)) {
                ++found;
                if (found == need)
                    return (seq_t)candidate;
            }
        }
        candidate += 2; /* only odd candidates */
    }

    /* The loop always returns; this line is only to satisfy the compiler. */
    return (seq_t)0;
}
