/*
 * A023981  Sum of exponents in prime-power factorization of multinomial
 *          coefficient M(5n; n,n,n,n,n).
 * https://oeis.org/A023981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over primes p and k>=1 of (⌊5n/p^k⌋ – 5·⌊n/p^k⌋)
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ---------- prime generation, incremental ---------- */

static idx_t _primes_limit = 1;      /* highest number checked for primality */
static size_t _primes_count = 0;     /* number of primes stored */
static uint32_t * _primes = NULL;    /* array of primes */

/* Ensure primes up to at least N inclusive are stored. */
static void _ensure_primes_upto(idx_t N)
{
    if (N <= _primes_limit) return;

    for (idx_t i = _primes_limit + 1; i <= N; ++i) {
        bool is_prime = true;
        for (size_t pi = 0; pi < _primes_count; ++pi) {
            uint32_t p = _primes[pi];
            if ((idx_t)p * (idx_t)p > i) break;
            if (i % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            uint32_t *tmp = (uint32_t *)realloc(_primes,
                                                (_primes_count + 1) * sizeof(uint32_t));
            if (!tmp) abort();            /* insufficient memory */
            _primes = tmp;
            _primes[_primes_count++] = (uint32_t)i;
        }
    }
    _primes_limit = N;
}

/* ---------- sequence computation ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t m = 5 * n;          /* 5·n */
    seq_t sum = 0;

    /* generate the primes needed for this n */
    _ensure_primes_upto(m);

    for (size_t pi = 0; pi < _primes_count; ++pi) {
        uint32_t p = _primes[pi];
        if (p > m) break;

        idx_t pow = (idx_t)p;                /* current power of p */
        while (pow <= m) {
            seq_t term_5n = (seq_t)(m / pow);    /* ⌊5n/p^k⌋ */
            seq_t term_n  = (seq_t)(n / pow);    /* ⌊n/p^k⌋  */
            sum += term_5n - (seq_t)5 * term_n;

            /* prepare next power, stop if it would overflow or exceed m */
            if (pow > m / (idx_t)p) break;
            pow *= (idx_t)p;
        }
    }
    return sum;
}
