/*
 * A162901  a(1)=1. For n >= 2, a(n) = the smallest integer >= a(n-1) such
 *          that gcd(n, a(n)) = p^k, where p = prime, k >= 1.
 * https://oeis.org/A162901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>=2, a(n)=the smallest integer >= a(n-1) such that
 *            gcd(n,a(n)) is a power of a prime (p^k, k>=1)
 * Range:     u8 n <= 87, u16 n <= 4903, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Static memoization storage. */
static seq_t *memo = NULL;
static idx_t memo_len = 0;

/* Ensure memo array can hold indices up to n. */
static void ensure_memo(idx_t n)
{
    if (n < memo_len) return;
    size_t new_len = (size_t)n + 1;
    seq_t *new_mem = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_mem) abort();
    if (memo) {
        memcpy(new_mem, memo, memo_len * sizeof(seq_t));
        free(memo);
    }
    memset(new_mem + memo_len, 0, (new_len - memo_len) * sizeof(seq_t));
    memo = new_mem;
    memo_len = (idx_t)new_len;
}

/* Return distinct prime factors of n (idx_t). */
static void distinct_prime_factors(idx_t n, idx_t *out, size_t *cnt)
{
    *cnt = 0;
    if (n % 2 == 0) {
        out[(*cnt)++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    for (idx_t f = 3; f * f <= n; f += 2) {
        if (n % f == 0) {
            out[(*cnt)++] = f;
            while (n % f == 0) n /= f;
        }
    }
    if (n > 1) out[(*cnt)++] = n;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* Undefined for n=0, safe return */
    ensure_memo(n);
    if (memo[n] != 0) return memo[n];

    if (n == 1) {
        memo[1] = 1;
        return 1;
    }

    /* Compute previous term recursively. */
    seq_t prev = a(n - 1);

    /* Factorize n. */
    idx_t primes[16];
    size_t pcnt;
    distinct_prime_factors(n, primes, &pcnt);

    seq_t best = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t p = primes[i];
        seq_t t = (prev + (seq_t)p - 1) / (seq_t)p;  /* ceil(prev/p) */

        /* Ensure t is not divisible by any other prime factor of n. */
        bool bad;
        do {
            bad = false;
            for (size_t j = 0; j < pcnt; ++j) {
                if (j == i) continue;
                if (t % (seq_t)primes[j] == 0) {
                    bad = true;
                    break;
                }
            }
            if (bad) ++t;
        } while (bad);

        seq_t candidate = (seq_t)p * t;
        if (best == 0 || candidate < best) best = candidate;
    }

    memo[n] = best;
    return best;
}
