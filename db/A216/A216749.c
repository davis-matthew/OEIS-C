/*
 * A216749  Primes p such that x^36 = -2 has no solution mod p.
 * https://oeis.org/A216749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A216749 – primes p for which x^36 = -2 has no solution modulo p.
 *
 * For an odd prime p, x^36 = a has a solution iff
 * a^((p-1)/gcd(p-1,36)) ≡ 1 (mod p).
 * Here a = -2 (mod p) = p-2.
 * Therefore we test: powmod(p-2, (p-1)/gcd(p-1,36), p) != 1.
 *
 * The function keeps an internal cache of found terms and generates
 * successive primes using next_prime until the requested index is reached.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/next_prime.h"

static idx_t processed_index = 0;          /* number of primes already tested */
static idx_t seed_prime = 3;                /* last prime tested (starting before 5) */

static seq_t *result_list = NULL;          /* stored sequence terms */
static size_t result_cnt = 0;              /* number of terms currently stored */
static size_t result_cap = 0;              /* capacity of result_list */

/* Ensure result_list can hold at least 'need' elements. */
static void ensure_result_capacity(size_t need)
{
    if (need <= result_cap)
        return;
    size_t new_cap = result_cap ? result_cap << 1 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(result_list, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failure – abort the program. */
        exit(1);
    }
    result_list = tmp;
    result_cap = new_cap;
}

/* Append a new term to result_list. */
static void append_result(seq_t val)
{
    ensure_result_capacity(result_cnt + 1);
    result_list[result_cnt++] = val;
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 1, but harmless */

    /* Generate primes and test until we have enough terms. */
    while (result_cnt < n) {
        /* Get next prime > 3. */
        seed_prime = next_prime(seed_prime);
        if (seed_prime == 0) /* no more primes (should never happen) */ {
            break;
        }
        if (seed_prime < 5) /* skip 2 and 3 */
            continue;

        seq_t p = (seq_t)seed_prime;
        seq_t m1 = p - 1;
        seq_t g = gcd(m1, (seq_t)36);
        seq_t exp = m1 / g;
        seq_t base = p - 2;   /* -2 modulo p */

        seq_t r = mod_pow(base, (uint64_t)exp, p);
        if (r != 1) {
            append_result(p);
        }
    }

    /* The sequence is cached, so return the required term. */
    return result_list[n - 1];
}
