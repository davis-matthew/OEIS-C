/*
 * A082542  a(n) = prime(n) + 2 - (prime(n) mod 4).
 * https://oeis.org/A082542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) + 2 - (prime(n) mod 4)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any n ≥ 1, using an incremental sieve of Eratosthenes
   that stores primes as seq_t values.  The algorithm is O(n^(3/2)) in the
   worst case but is fast enough for n up to about 10 000. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Dynamic storage for primes as seq_t values (0‑based index). */
static seq_t *prime_tbl = NULL;
static idx_t prime_cnt = 0;   /* number of primes currently stored */
static idx_t prime_cap = 0;   /* current capacity of prime_tbl */

/* Ensure prime_tbl can hold at least 'need' primes. */
static void ensure_capacity(idx_t need)
{
    if (prime_cap >= need)
        return;
    idx_t new_cap = prime_cap ? prime_cap << 1 : need;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(prime_tbl, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* out of memory */
    prime_tbl = tmp;
    prime_cap = new_cap;
}

/* Grow the prime table until it contains at least the first 'n' primes. */
static void fill_primes(idx_t n)
{
    if (prime_cnt >= n)
        return;

    /* Prime list is empty: insert 2 as the first prime. */
    if (prime_cnt == 0) {
        ensure_capacity(1);
        prime_tbl[prime_cnt++] = (seq_t)2;
    }

    seq_t candidate = prime_tbl[prime_cnt - 1] + (seq_t)1;
    /* Make sure we start with an odd candidate. */
    if ((candidate & (seq_t)1) == 0)
        candidate += (seq_t)1;

    /* Simple incremental primality test using trial division by stored primes. */
    while (prime_cnt < n) {
        bool is_prime = true;
        for (idx_t i = 0; i < prime_cnt; ++i) {
            seq_t p = prime_tbl[i];
            /* If p*p > candidate, remaining divisors are too large. */
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            ensure_capacity(prime_cnt + 1);
            prime_tbl[prime_cnt++] = candidate;
        }
        candidate += (seq_t)2;   /* skip even numbers */
    }
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* behaviour below offset is irrelevant */

    fill_primes(n);           /* ensure that prime(n) exists */
    seq_t p = prime_tbl[n - 1];
    seq_t rem = p % (seq_t)4;
    return p + (seq_t)2 - rem;
}
