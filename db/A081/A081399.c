/*
 * A081399  Bigomega of the n-th Catalan number: a(n) = A001222(A000108(n)).
 * https://oeis.org/A081399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Ω( Catalan(n) ) = Ω((2n)!)-2·Ω(n!)-Ω(n+1) (Ω: big‑omega,
 *            number of prime factors counted with multiplicity)
 * Range:     u8 n <= 1201, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime table ---------- */
static idx_t *prime_arr = NULL;   /* array of primes */
static size_t prime_cnt = 0;      /* number of primes stored */
static idx_t  prime_limit = 0;    /* largest number processed */

/* Grow the prime array up to at least 'limit' and fill it with new primes. */
static void
ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Allocate space for all numbers up to limit; it is more than enough for primes. */
    size_t cap = (size_t)(limit + 1);
    idx_t *new_arr = (idx_t *)realloc(prime_arr, cap * sizeof(idx_t));
    if (!new_arr)
        abort();                /* allocation failed */
    prime_arr = new_arr;

    /* Add new primes from prime_limit+1 up to limit. */
    if (prime_limit < 2)
        prime_limit = 1;        /* start from 2 */

    for (idx_t cand = prime_limit + 1; cand <= limit; ++cand) {
        bool is_p = true;
        /* Test divisibility only by existing primes up to sqrt(cand). */
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_arr[i];
            if (p * p > cand)          /* p > sqrt(cand) */
                break;
            if (cand % p == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            prime_arr[prime_cnt++] = cand;
        }
    }
    prime_limit = limit;
}

/* ---------- Big‑omega of a factorial ---------- */
static seq_t
bigomega_factorial(idx_t n)
{
    if (n < 2)
        return 0;

    ensure_primes_upto(n);
    seq_t total = 0;

    for (size_t i = 0; i < prime_cnt && prime_arr[i] <= n; ++i) {
        idx_t p = prime_arr[i];
        idx_t power = p;
        seq_t exp = 0;

        while (power <= n) {
            exp += n / power;
            if (power > n / p)          /* avoid overflow of power*p */
                break;
            power *= p;
        }
        total += exp;
    }
    return total;
}

/* ---------- Big‑omega of an arbitrary integer ---------- */
static seq_t
bigomega_integer(idx_t m)
{
    if (m < 2)
        return 0;

    ensure_primes_upto(m);
    seq_t total = 0;

    for (size_t i = 0; i < prime_cnt && prime_arr[i] <= m; ++i) {
        idx_t p = prime_arr[i];
        while (m % p == 0) {
            ++total;
            m /= p;
        }
        if (m == 1)
            break;
    }
    if (m > 1)               /* m is a remaining prime factor */
        ++total;
    return total;
}

/* ---------- Main sequence generator ---------- */
seq_t
a(idx_t n)
{
    /* Special cases handled naturally, but explicit check improves readability. */
    if (n == 0 || n == 1)
        return 0;

    /* We need primes up to 2n to compute factorial exponents. */
    ensure_primes_upto(2 * n);

    seq_t f2n = bigomega_factorial(2 * n);
    seq_t fn  = bigomega_factorial(n);
    seq_t omega_n1 = bigomega_integer(n + 1);

    /* Ω(C_n) = Ω((2n)!)-2·Ω(n!)-Ω(n+1) */
    return f2n - (seq_t)(2 * fn) - omega_n1;
}
