/*
 * A054821  Third term of weak prime quartet: p(m-1)-p(m-2) < p(m)-p(m-1) <
 *          p(m+1)-p(m).
 * https://oeis.org/A054821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A054821: Third term of weak prime quartet
   Formula: Find primes p_m such that
            p_{m-1}-p_{m-2} < p_m-p_{m-1} < p_{m+1}-p_m.
   The function returns the m‑th such prime. It keeps an on‑demand list of
   primes and of the valid third terms. */

/* The build supplies seq_t and idx_t; we do not redeclare them. */
#include <stdlib.h>
#include <stddef.h>

/* dynamic array for primes */
static seq_t *primes = NULL;
static size_t prime_cap = 0, prime_len = 0;

/* dynamic array for the third terms that satisfy the condition */
static seq_t *weak = NULL;
static size_t weak_cap = 0, weak_len = 0;

/* push a prime into the primes array, growing it when necessary */
static void push_prime(seq_t p)
{
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap == 0 ? 256 : prime_cap * 2;
        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        primes = tmp; prime_cap = new_cap;
    }
    primes[prime_len++] = p;
}

/* push a weak term into the weak array */
static void push_weak(seq_t v)
{
    if (weak_len == weak_cap) {
        size_t new_cap = weak_cap == 0 ? 128 : weak_cap * 2;
        seq_t *tmp = (seq_t *)realloc(weak, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        weak = tmp; weak_cap = new_cap;
    }
    weak[weak_len++] = v;
}

/* trial division primality test using the primes we have seen so far */
static int is_prime_candidate(seq_t n)
{
    for (size_t i = 0; i < prime_len; ++i) {
        seq_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

/* generate primes and weak terms until at least 'target' weak terms have
   been found.  The function can be called repeatedly; it stops as soon
   as the requested amount is satisfied. */
static void ensure_n_weak_terms(idx_t target)
{
    if (weak_len >= target) return;

    /* initialise the prime list if it is empty */
    if (prime_len == 0) {
        push_prime(2);
        push_prime(3);
    }
    if (prime_len == 2) push_prime(5);

    seq_t candidate = primes[prime_len - 1] + 2;  /* next odd number */

    while (weak_len < target) {
        if (is_prime_candidate(candidate)) {
            push_prime(candidate);

            /* check for a quartet: need at least 4 primes */
            if (prime_len >= 4) {
                seq_t g1 = primes[prime_len - 3] - primes[prime_len - 4];
                seq_t g2 = primes[prime_len - 2] - primes[prime_len - 3];
                seq_t g3 = primes[prime_len - 1] - primes[prime_len - 2];
                if (g1 < g2 && g2 < g3) {
                    /* third term of quartet */
                    push_weak(primes[prime_len - 2]);
                }
            }
        }
        candidate += 2;
    }
}

/* required exported function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined for n==0 */
    ensure_n_weak_terms(n);
    return weak[n - 1];
}
