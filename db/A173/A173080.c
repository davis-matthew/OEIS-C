/*
 * A173080  Positive integers with the property that the sum of the cubes of
 *          their divisors is a multiple of 585.
 * https://oeis.org/A173080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173080: integers n such that σ₃(n) is a multiple of 585.
   The function searches integers in order and returns the n‑th such
   integer (offset 1). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- prime sieve (once) --------------------------------- */
enum { PRIME_LIMIT = 100000 };          /* should cover all needed primes */
static bool prime_loaded = false;
static uint32_t primes[PRIME_LIMIT];
static size_t prime_count = 0;

static void load_primes(void)
{
    if (prime_loaded) return;
    bool *is_prime = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);   /* fatal error */
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;
    prime_count = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) primes[prime_count++] = (uint32_t)i;
    free(is_prime);
    prime_loaded = true;
}

/* ---------- test whether σ₃(n) ≡ 0 (mod 585) ------------------- */
static inline bool good_n(idx_t n)
{
    uint64_t temp = (uint64_t)n;
    uint64_t cur   = 1;                   /* product modulo 585 */

    for (size_t i = 0; i < prime_count &&
         (uint64_t)primes[i] * primes[i] <= temp; ++i)
    {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) { temp /= p; ++exp; }

            /* compute (1 + p³ + p⁶ + … + p^{3·exp}) mod 585 */
            uint64_t term = 1;
            uint64_t base = 1;
            uint64_t pmod = (uint64_t)p % 585;
            uint64_t p3   = (pmod * pmod % 585) * pmod % 585;

            for (int k = 1; k <= exp; ++k) {
                base = (base * p3) % 585;
                term = (term + base) % 585;
            }
            cur = (cur * term) % 585;
            if (cur == 0) return true;
        }
    }
    if (temp > 1) {                          /* remaining prime factor */
        uint64_t pmod = temp % 585;
        uint64_t p3   = (pmod * pmod % 585) * pmod % 585;
        uint64_t term = (1 + p3) % 585;
        cur = (cur * term) % 585;
    }
    return cur == 0;
}

/* ---------- main sequence function ----------------------------- */
seq_t a(idx_t n)
{
    /* initialise prime list once */
    load_primes();

    idx_t count = 0;
    idx_t candidate = 8;                     /* first known member */
    while (true) {
        if (good_n(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
