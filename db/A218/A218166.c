/*
 * A218166  a(n) is the smallest positive integer k such that k^256 + 1 == 0
 *          mod p, where p is the n-th prime of the form 1 + 512*b (see
 *          A076339).
 * https://oeis.org/A218166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) is the smallest positive integer k such that k^256 + 1 ≡ 0
 *            (mod p), where p is the n‑th prime of the form 1 + 512*b (i.e. p
 *            ≡ 1 (mod 512))
 * Range:     u8 none, u16 n <= 24, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* is_prime(idx_t n) */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */

static uint64_t *primes1 = NULL;   /* array of primes ≡ 1 (mod 512) */
static size_t primes_len = 0;      /* number of primes stored */
static size_t primes_cap = 0;      /* current capacity of the array */
static uint64_t next_candidate = 1ULL; /* last checked candidate (1 mod 512) */

/* Ensure that primes1 has at least n entries */
static void ensure_primes_for_n(idx_t n)
{
    if (primes_len >= (size_t)n)
        return;

    if (primes1 == NULL) {
        primes_cap = 64;
        primes1    = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes1)
            abort();
    }

    while (primes_len < (size_t)n) {
        /* advance to next candidate of the form 1 mod 512 */
        do {
            next_candidate += 512ULL;
        } while (!is_prime(next_candidate));

        /* store the found prime */
        if (primes_len == primes_cap) {
            primes_cap *= 2;
            uint64_t *tmp = (uint64_t *)realloc(primes1, primes_cap * sizeof(uint64_t));
            if (!tmp)
                abort();
            primes1 = tmp;
        }
        primes1[primes_len++] = next_candidate;
    }
}

/* OEIS sequence A218166 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* undefined for n = 0; return 0 safely */

    ensure_primes_for_n(n);
    uint64_t p = primes1[n - 1];
    seq_t mod  = (seq_t)p;
    seq_t target = mod - (seq_t)1;   /* -1 modulo p */

    /* Search for the minimal k with k^256 ≡ -1 (mod p) */
    for (idx_t k = 1; ; ++k) {
        seq_t pow = mod_pow((seq_t)k, 256ULL, mod);
        if (pow == target)
            return (seq_t)k;   /* cast back to seq_t */
    }
}
