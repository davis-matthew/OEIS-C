/*
 * A182922  a(n) = n! / A055773(n).
 * https://oeis.org/A182922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{p ≤ n, vp(n!) ≥ 2} p^vp(n!) (powerful part of
 *            n!)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t primes[2000];   /* primes up to 10000 – 1229 primes */
static size_t prime_cnt = 0;    /* number of primes stored    */
static idx_t primes_limit = 0;  /* largest n for which primes are up to date */

/* Build the list of primes up to n (incrementally). */
static void init_primes(idx_t n)
{
    if (n <= primes_limit) return;

    for (idx_t x = (primes_limit ? primes_limit + 1 : 2); x <= n; ++x) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > x) break;
            if (x % p == 0) { is_prime = false; break; }
        }
        if (is_prime && prime_cnt < 2000) primes[prime_cnt++] = x;
    }
    primes_limit = n;
}

/* fast exponentiation for seq_t base raised to an idx_t exponent */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Return the a(n) value of OEIS A182922. */
seq_t a(idx_t n)
{
    init_primes(n);

    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt && primes[i] <= n; ++i) {
        uint64_t p = primes[i];

        /* Legendre’s formula: exponent of prime p in n! */
        idx_t e = 0;
        idx_t t = n;
        while (t) {
            t /= (idx_t)p;
            e += t;
        }

        if (e <= 1) continue;                /* skip primes with exponent < 2 */
        seq_t power = pow_seq((seq_t)p, e);
        result *= power;
    }
    return result;
}
