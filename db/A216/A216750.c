/*
 * A216750  Primes p such that x^38 = -2 has no solution mod p.
 * https://oeis.org/A216750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A216750: primes p such that x^38 = -2 has no solution mod p.
   The function returns the n‑th such prime (offset = 1). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "../utils/is_prime.h"
#include "../utils/gcd_u64.h"

static seq_t *seq_cache = NULL;
static size_t seq_cnt = 0, seq_cap = 0;
static uint64_t last_prime = 3;          /* start search after 3 */

/* Grow the cache array for the sequence. */
static void grow_seq(void)
{
    if (seq_cap == 0) {
        seq_cap = 8;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    } else {
        seq_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
}

/* Greatest common divisor for 64‑bit integers. */

/* Fast modular exponentiation for 64‑bit values. */
static uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return result;
}

/* Check whether x^38 ≡ -2 (mod p) has no solution. */
static bool no_solution(uint64_t p)
{
    if (p <= 2) return false;                /* not in sequence */
    uint64_t g = gcd_u64(38ULL, p - 1);
    uint64_t exp = (p - 1) / g;
    uint64_t rhs = modpow_uint64(p - 2, exp, p);   /* (-2)^exp mod p */
    return rhs != 1;
}

/* Generate the next prime > last_prime, test it, and store it if it
   satisfies the property. */
static void generate_next_prime(void)
{
    uint64_t cand = last_prime + 1;
    if (cand <= 2) cand = 3;
    if ((cand & 1ULL) == 0) cand++;           /* skip even numbers */

    for (;;) {
        if (is_prime((idx_t)cand)) {          /* use helper primality test */
            last_prime = cand;
            if (no_solution(cand)) {
                if (seq_cnt == seq_cap) grow_seq();
                seq_cache[seq_cnt++] = (seq_t)cand;
            }
            return;
        }
        cand += 2;  /* next odd candidate */
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* the sequence is 1‑indexed */

    while (seq_cnt < (size_t)n) {
        generate_next_prime();
    }
    return seq_cache[n - 1];
}
