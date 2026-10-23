/*
 * A176695  Primes of the form x^2 + 29*(x+1)^2.
 * https://oeis.org/A176695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 1349, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, mod_mul, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS A176695: primes of the form x^2 + 29*(x+1)^2.
   The n‑th such prime (with a(1)=29) is returned. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/powmod64.h"
#include "../utils/mod_mul.h"

static const uint32_t small_primes[] = {
    2u,3u,5u,7u,11u,13u,17u,19u,23u,29u,
    31u,37u,41u,43u,47u,53u,59u,61u,67u,71u,
    73u,79u,83u,89u,97u,101u,103u,107u,109u,113u
};
static const size_t small_cnt = sizeof(small_primes)/sizeof(small_primes[0]);

/* Deterministic Miller–Rabin for 64‑bit integers
   (suffices for all numbers in this sequence up to n≈10000). */
static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < small_cnt; ++i) {
        uint32_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2ull,3ull,5ull,7ull,11ull,13ull};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined case, return 0 */

    seq_t x = 0;           /* current index x */
    idx_t  count = 0;      /* primes found so far */

    while (true) {
        /* compute p = x^2 + 29*(x+1)^2 = 30*x^2 + 58*x + 29 */
        seq_t xsq  = x * x;
        seq_t term = xsq * 30 + x * 58 + 29;

        /* quick small‑prime filtering */
        bool prime = true;
        for (size_t i = 0; i < small_cnt; ++i) {
            uint32_t p = small_primes[i];
            if (term == (seq_t)p) {                     /* term itself is a small prime */
                prime = true; break;
            }
            if ((term % p) == 0) {                      /* divisible by a small prime */
                prime = false; break;
            }
        }

        /* if still undetermined, use deterministic test */
        if (prime) {
            /* if term fits in 64 bits, use fast 64‑bit primality test */
            if (term <= (seq_t)UINT64_MAX) {
                uint64_t p64 = (uint64_t)term;
                prime = is_prime64(p64);
            } else {
                /* otherwise fall back to the generic bigint primality test */
                prime = is_prime_big(term);
            }
        }

        if (prime) {
            ++count;
            if (count == n) return term;
        }
        ++x;     /* next candidate */
    }
}
