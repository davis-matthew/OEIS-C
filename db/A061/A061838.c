/*
 * A061838  Numbers k such that (k+3)^3 | k!.
 * https://oeis.org/A061838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061838: numbers k such that (k+3)^3 divides k!.
   Search by testing integers sequentially, using Legendre’s
   formula to verify the divisibility.  The routine memoises
   found terms for fast repeated queries.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"   /* for grow_seq() */

/* ---------- prime sieve for factorisation of (k+3) ---------- */
#define MAX_PRIME 1000000
static unsigned char is_prime[MAX_PRIME + 1];
static uint32_t primes[80000];
static size_t prime_cnt = 0;
static bool primes_initialised = false;

static void init_primes(void)
{
    if (primes_initialised)
        return;
    size_t i, j;
    for (i = 0; i <= MAX_PRIME; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (i = 2; i * i <= MAX_PRIME; ++i)
        if (is_prime[i])
            for (j = i * i; j <= MAX_PRIME; j += i)
                is_prime[j] = 0;
    prime_cnt = 0;
    for (i = 2; i <= MAX_PRIME; ++i)
        if (is_prime[i])
            primes[prime_cnt++] = (uint32_t)i;
    primes_initialised = true;
}

/* ---------- Legendre exponent of prime p in k! ---------- */
static uint64_t v_p_factorial(uint64_t k, uint32_t p)
{
    uint64_t exp = 0;
    uint64_t pp  = p;
    while (pp <= k) {
        exp += k / pp;
        if (pp > k / p)   /* avoid overflow of pp *= p */
            break;
        pp *= p;
    }
    return exp;
}

/* ---------- check whether (k+3)^3 divides k! ---------- */
static bool ok_k(uint64_t k)
{
    init_primes();
    uint64_t x = k + 3;
    uint64_t tmp = x;
    size_t idx = 0;
    for (; idx < prime_cnt && (uint64_t)primes[idx] * (uint64_t)primes[idx] <= tmp; ++idx) {
        uint32_t p = primes[idx];
        if (tmp % p == 0) {
            unsigned e = 0;
            do {
                tmp /= p;
                ++e;
            } while (tmp % p == 0);
            if (v_p_factorial(k, p) < 3ULL * e)
                return false;
        }
    }
    if (tmp > 1) {   /* remaining prime factor */
        uint32_t p = (uint32_t)tmp;
        if (v_p_factorial(k, p) < 3ULL)
            return false;
    }
    return true;
}

/* ---------- memoised term storage ---------- */
static seq_t *terms      = NULL;
static size_t terms_len  = 0;
static size_t terms_cap  = 0;
static uint64_t last_k   = 8;   /* next candidate will be 9 */

/* Ensure at least n terms have been generated. */
static void ensure_terms(idx_t n)
{
    while (terms_len < n) {
        ++last_k;
        if (ok_k(last_k)) {
            if (terms_len + 1 > terms_cap)
                grow_seq(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = (seq_t)last_k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but harmless */
    ensure_terms(n);
    return terms[n - 1];
}
