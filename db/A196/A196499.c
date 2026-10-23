/*
 * A196499  Numbers k such that the greatest residue of the congruence x^k
 *          (mod k) equals k-1 for x in [0..k-1].
 * https://oeis.org/A196499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A196499 – numbers k such that the congruence x^k ≡ −1 (mod k) *
 *            has a solution. This is equivalent to “−1 is a k‑th power
 *            residue * modulo k”. An integer k satisfies the condition iff *
 *            * (a) k is not divisible by 4, and * (b) for every odd prime
 *            power p^e dividing k, * gcd(k, φ(p^e)) | φ(p^e)/2....
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* --- Small prime sieve -------------------------------------------------- */
#define PRIME_LIMIT 200000         /* enough for k up to several hundred‑thousand */
static uint32_t primes[PRIME_LIMIT];
static size_t prime_count = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    bool *is_prime = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime) return;                 /* allocation failure – abort silently */
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i) is_prime[j] = false;
        }
    }
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) primes[prime_count++] = (uint32_t)i;
    free(is_prime);
    primes_ready = true;
}

/* --- Test whether a given k satisfies the property --------------------- */
static bool satisfies(idx_t k)
{
    if (k == 1) return true;               /* 1 is always included     */
    if (k % 4 == 0) return false;          /* multiples of 4 fail      */

    idx_t tmp = k;
    /* factor all odd primes (skip p=2)                                     */
    for (size_t i = 0; i < prime_count && (idx_t)primes[i] * (idx_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            /* count exponent e of p in k */
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }

            /* φ(p^e) = p^(e−1)(p−1) */
            seq_t phi = 1;
            for (idx_t j = 0; j < e - 1; ++j) phi *= (seq_t)p;
            phi *= (seq_t)(p - 1);

            seq_t half = phi / 2;
            seq_t g = gcd((seq_t)k, phi);
            if (half % g) return false;    /* condition (b) fails  */
        }
    }

    /* any remaining prime factor > sqrt(k) */
    if (tmp > 1) {
        seq_t phi = (seq_t)(tmp - 1);       /* e = 1  */
        seq_t half = phi / 2;
        seq_t g = gcd((seq_t)k, phi);
        if (half % g) return false;
    }
    return true;                            /* all conditions satisfied */
}

/* --- Main sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    init_primes();
    idx_t count = 0;
    idx_t k = 1;
    while (true) {
        if (satisfies(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
}
