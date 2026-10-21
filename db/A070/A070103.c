/*
 * A070103  Number of obtuse integer triangles with perimeter n and prime side
 *          lengths.
 * https://oeis.org/A070103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of obtuse integer triangles with perimeter n whose
 *            three side lengths are prime numbers. Triangles are counted up
 *            to permutation (i.e., (p,q,r) with p≤q≤r). The function uses a
 *            simple sieve to generate primes up to n and then checks all
 *            prime triples a≤b≤c that satisfy the triangle...
 * Range:     u8 n <= 34, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* global state for prime sieve and list */
static idx_t current_limit = 0;      /* maximum n already sieved */
static bool *is_prime      = NULL;   /* primality flag array up to current_limit */
static idx_t *prime_list  = NULL;   /* sorted list of primes ≤ current_limit */
static size_t prime_count = 0;       /* number of primes in prime_list */

/* extend the sieve and prime list to at least n */
static void build_primes_up_to(idx_t n)
{
    if (n <= current_limit) return;   /* already sieved */

    /* allocate new sieve */
    size_t size = (size_t)n + 1;
    bool *new_sieve = (bool*)calloc(size, sizeof(bool));
    if (!new_sieve) abort();
    memset(new_sieve, 1, size);           /* assume all are prime */
    new_sieve[0] = new_sieve[1] = false;  /* 0 and 1 are not prime */

    /* sieve of Eratosthenes */
    for (idx_t i = 2; (idx_t)(i * i) <= n; ++i) {
        if (new_sieve[i]) {
            for (idx_t j = i * i; j <= n; j += i) new_sieve[j] = false;
        }
    }

    /* count primes and build list */
    size_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i) if (new_sieve[i]) ++cnt;

    idx_t *new_list = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!new_list) abort();
    size_t pos = 0;
    for (idx_t i = 2; i <= n; ++i) if (new_sieve[i]) new_list[pos++] = i;

    /* replace old data */
    free(is_prime);
    free(prime_list);
    is_prime   = new_sieve;
    prime_list = new_list;
    prime_count = cnt;
    current_limit = n;
}

/* main sequence routine */
seq_t a(idx_t n)
{
    if (n < 6) return 0;    /* minimal perimeter for any triangle with primes */

    /* ensure sieve covers n */
    build_primes_up_to(n);

    seq_t count = 0;

    /* iterate over primes a ≤ b ≤ c */
    for (size_t ia = 0; ia < prime_count; ++ia) {
        idx_t a_val = prime_list[ia];
        if (a_val > n) break;

        for (size_t ib = ia; ib < prime_count; ++ib) {
            idx_t b_val = prime_list[ib];
            idx_t ab = a_val + b_val;
            if (ab > n) break;            /* no room for c */

            idx_t c_val = n - ab;
            if (c_val < b_val) break;     /* maintain a ≤ b ≤ c */

            /* triangle inequality: a + b > c */
            if (ab <= c_val) continue;

            /* c must be prime (cms already sieved) */
            if (!is_prime[c_val]) continue;

            /* obtuse condition: c^2 > a^2 + b^2 */
            seq_t c2 = (seq_t)c_val * (seq_t)c_val;
            seq_t sum_sq = (seq_t)a_val * (seq_t)a_val
                         + (seq_t)b_val * (seq_t)b_val;
            if (c2 > sum_sq) ++count;
        }
    }

    return count;
}
