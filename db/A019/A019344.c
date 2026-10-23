/*
 * A019344  Primes with primitive root 17.
 * https://oeis.org/A019344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth prime p for which 17 is a primitive root modulo p */

/*  The function scans primes sequentially, testing the primitive‑root
 *  condition by checking that 17^((p-1)/q) ≠ 1 (mod p) for each prime
 *  divisor q of p-1.  It is written to use only the allowed utilities
 *  and works for any seq_t / idx_t types. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

#define SMALL_PRIME_LIMIT 100000   /* enough for all primes needed up to 10 000 terms */

/* --------------------------------------------------------------------- */
/*  Small prime table, built once on first use.                          */
/* --------------------------------------------------------------------- */
static uint64_t small_primes[SMALL_PRIME_LIMIT]; /* over‑alloc – safe */
static size_t small_cnt = 0;

static void init_small_primes(void)
{
    if (small_cnt != 0) return;          /* already built */

    /* simple sieve to generate all primes < SMALL_PRIME_LIMIT */
    bool is_composite[SMALL_PRIME_LIMIT] = { false };
    size_t i, j;
    for (i = 2; i < SMALL_PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (small_cnt < SMALL_PRIME_LIMIT)
                small_primes[small_cnt++] = i;
            for (j = i * i; j < SMALL_PRIME_LIMIT; j += i)
                is_composite[j] = true;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Test whether 17 is a primitive root modulo the prime p.              */
/* --------------------------------------------------------------------- */
static bool primitive_root_17(uint64_t p)
{
    if (p == 17) return false;   /* exclude the trivial case */

    uint64_t phi = p - 1;
    uint64_t remaining = phi;

    for (size_t i = 0; i < small_cnt && small_primes[i] * small_primes[i] <= remaining; ++i) {
        uint64_t q = small_primes[i];
        if (remaining % q == 0) {
            /* Test the condition for this prime factor q. */
            seq_t r = mod_pow((seq_t)17, (uint64_t)(phi / q), (seq_t)p);
            if (r == (seq_t)1) return false;
            while (remaining % q == 0) remaining /= q;
        }
    }
    if (remaining > 1) {
        /* remaining is now the last prime factor of phi (p-1). */
        seq_t r = mod_pow((seq_t)17, (uint64_t)(phi / remaining), (seq_t)p);
        if (r == (seq_t)1) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Return the nth prime (1‑based) that has 17 as a primitive root.     */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0; return 0 */

    init_small_primes();

    uint64_t candidate = 2;        /* first prime to test */
    idx_t found = 0;               /* number of suitable primes found */

    while (found < n) {
        if (is_prime(candidate)) {
            if (primitive_root_17(candidate)) {
                ++found;
                if (found == n) return (seq_t)candidate;
            }
        }
        ++candidate;
    }

    /* unreachable, but keep compiler happy */
    return 0;
}
