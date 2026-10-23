/*
 * A040141  Primes p such that x^4 = 18 has a solution mod p.
 * https://oeis.org/A040141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that there exists an integer x with
   x^4 ≡ 18 (mod p).  For p = 2,3 the congruence holds trivially
   because 18 ≡ 0 (mod p) and 0^4 ≡ 0.  For odd primes p ∤ 18,
   the condition is equivalent to 18 being a fourth‑power residue
   modulo p, i.e. 18^((p‑1)/gcd(4,p–1)) ≡ 1 (mod p).  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* ---------- Cache for sequence values ---------- */
static seq_t *seq_cache = NULL;
static size_t   seq_cap  = 0;
static size_t   seq_len  = 0;

/* ---------- Cache for small primes used by trial division ---------- */
static uint64_t *primes      = NULL;
static size_t    primes_cap  = 0;
static size_t    primes_used = 0;
static uint64_t  next_candidate = 3;   /* first odd candidate */

/* Ensure seq_cache holds at least `need` elements.  Casts are needed for C++. */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Ensure primes array holds at least `need` primes. */
static void ensure_primes_capacity(size_t need)
{
    if (need <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    primes_cap = new_cap;
}

/* Trial division test using cached primes. */
static bool is_prime_candidate(uint64_t n)
{
    for (size_t i = 0; i < primes_used; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Produce the next prime after those already cached. */
static uint64_t next_prime(void)
{
    if (seq_len == 0) {
        /* 2 is inserted only once; primes[0] already set during init. */
        return 2;
    }
    while (true) {
        if (is_prime_candidate(next_candidate)) {
            uint64_t p = next_candidate;
            ensure_primes_capacity(primes_used + 1);
            primes[primes_used++] = p;
            next_candidate += 2;
            return p;
        }
        next_candidate += 2;
    }
}

/* Sequence a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe guard */

    if (seq_len < n) {
        ensure_seq_capacity(n);
        if (seq_len == 0) {               /* first call: seed 2 */
            seq_cache[0] = (seq_t)2;
            seq_len      = 1;
            ensure_primes_capacity(1);
            primes[0]      = 2;
            primes_used    = 1;
        }

        /* Generate primes and test condition until n terms found */
        while (seq_len < n) {
            uint64_t p = next_prime();     /* p is odd for all but 2 */

            /* Accept p = 2 or 3 trivially (18 ≡ 0 (mod p)). */
            if (p == 3) {
                seq_cache[seq_len++] = (seq_t)p;
                continue;
            }

            /* For primes p ∤ 18 : use fourth‑power residue test. */
            uint64_t e;
            if ((p - 1) % 4 == 0)
                e = (p - 1) / 4;
            else
                e = (p - 1) / 2;

            seq_t pow_val = mod_pow((seq_t)18, e, (seq_t)p);
            if (pow_val == (seq_t)1) {
                seq_cache[seq_len++] = (seq_t)p;
            }
        }
    }

    return seq_cache[n - 1];
}
