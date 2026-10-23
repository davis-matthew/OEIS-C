/*
 * A181794  Numbers n such that the number of even divisors of n is an even
 *          divisor of n.
 * https://oeis.org/A181794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 9599, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generation of A181794: numbers n such that the number of even divisors
   of n is an even divisor of n.  The implementation searches over even
   integers, computes the prime‑power factorisation of the odd part,
   obtains the divisor count τ(m), and checks the defining condition.
   It does not rely on any custom data structures so it works for both
   fixed‑width integer seq_t (up to 128 bits) and for arbitrary‑precision
   seq_t employed by the big‑integer build. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Limit for pre‑computed primes – sufficient for the first 10000
   sequence terms (the 10000th term is < 300000). */
#define PRIME_LIMIT 200000

/* Static storage for the list of primes produced by the sieve. */
static uint32_t primes[PRIME_LIMIT + 1];
static size_t prime_cnt = 0;
static bool primes_inited = false;

/* Simple sieve of Eratosthenes; fills the primes array. */
static void init_primes(void)
{
    if (primes_inited) return;
    bool is_comp[PRIME_LIMIT + 1] = {false};
    for (uint32_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = true;
        }
    }
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
        }
    }
    primes_inited = true;
}

/* Checks whether a given even unsigned 64‑bit integer satisfies the
   defining property of the sequence.  We factor out powers of two,
   compute τ(m) for the odd part m, form E = k * τ(m), and test that
   E is even and divides the number. */
static bool satisfies_property(uint64_t n)
{
    /* n is guaranteed to be even when called. */
    uint64_t m = n;
    uint64_t k = 0;
    while ((m & 1ULL) == 0) {
        ++k;
        m >>= 1;
    }

    /* Compute τ(m).  The odd part m fits into uint64_t for the
       required range of n. */
    uint64_t tau = 1;
    uint64_t tmp = m;
    size_t idx = 0;
    while (idx < prime_cnt && (uint64_t)primes[idx] * (uint64_t)primes[idx] <= tmp) {
        uint32_t p = primes[idx];
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            tau = tau * (exp + 1);
        }
        ++idx;
    }
    if (tmp > 1) {
        /* Remaining prime factor with exponent 1. */
        tau *= 2;
    }

    uint64_t E = k * tau;
    if (E == 0 || (E & 1ULL) != 0) return false;  /* need E even */

    return (n % E) == 0;
}

/* Main function requested by the problem statement.  It returns the
   n‑th term (1‑based) of the sequence.  The function recomputes the
   sequence from scratch on each call – this guarantees correctness
   irrespective of call order and keeps the implementation short and
   clear. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    init_primes();

    idx_t found = 0;
    uint64_t cand = 4;   /* the first term of the sequence is 4 */
    for (;; cand += 2) {   /* only even numbers can qualify */
        if (satisfies_property(cand)) {
            ++found;
            if (found == n) return (seq_t)cand;
        }
    }
}
