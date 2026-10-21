/*
 * A053580  Primes having only {0, 6, 8, 9} as digits.
 * https://oeis.org/A053580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 8765, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_allowed, mulmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate primes whose decimal digits belong to {0,6,8,9} and
   whose last digit is 9. The first prime is 89.  
   The function builds the sequence lazily and memoises the results. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digits_allowed.h"
#include "../utils/mulmod64.h"

static uint64_t *prime_list = NULL;
static size_t prime_len   = 0;
static size_t prime_cap   = 0;
static uint64_t current   = 89; /* next candidate to test */

/* multiplication modulo for 64‑bit numbers using 128‑bit intermediate */

/* binary exponentiation modulo */
static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = mulmod64(res, base, mod);
        base = mulmod64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* deterministic Miller‑Rabin for 64‑bit integers */
static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a >= n) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* expand the memoised list until it contains at least n elements */
static void ensure_n(idx_t n)
{
    if (prime_list == NULL) {
        prime_cap = 256;
        prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    }
    while (prime_len < n) {
        uint64_t cand = current;
        current += 10; /* keep last digit 9 */
        if (cand < 89) continue; /* skip values before first term */
        if (digits_allowed(cand) && is_prime64(cand)) {
            if (prime_len == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_len++] = cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, behaviour at 0 is irrelevant */
    ensure_n(n);
    return (seq_t)prime_list[n - 1];
}
