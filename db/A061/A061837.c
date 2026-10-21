/*
 * A061837  Numbers k such that (k+2)^2 | k!.
 * https://oeis.org/A061837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   satisfies_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061837: Numbers k such that (k+2)^2 divides k! */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>   /* for sqrt() */
#include "../utils/satisfies_prime.h"

/* static storage for the sequence and auxiliary data */
static seq_t *seq_terms    = NULL;   /* array of found terms */
static idx_t  seq_len      = 0;      /* number of terms stored */
static idx_t  seq_cap      = 0;      /* capacity of seq_terms */

static int  *prime_list   = NULL;   /* primes used for factorization */
static size_t prime_cap   = 0;       /* allocated size of prime_list */
static size_t prime_cnt   = 0;       /* number of primes stored */
static int   prime_limit  = 0;       /* largest prime already generated */

static idx_t next_k        = 1;     /* next k to test */

/* Ensure the prime list has primes up to at least 'limit' (inclusive). */
static void extend_primes(int limit)
{
    if (limit <= prime_limit) return;

    int new_limit = limit;
    bool *is_prime = (bool*)malloc((new_limit + 1) * sizeof(bool));
    if (!is_prime) { return; } /* allocation failed – unlikely */

    for (int i = 0; i <= new_limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= new_limit; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= new_limit; j += i) {
                is_prime[j] = false;
            }
        }
    }

    for (int i = prime_limit + 1; i <= new_limit; ++i) {
        if (is_prime[i]) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_list = (int*)realloc(prime_list, prime_cap * sizeof(int));
            }
            prime_list[prime_cnt++] = i;
        }
    }

    prime_limit = new_limit;
    free(is_prime);
}

/* Test whether (k+2)^2 divides k!. */
static bool k_satisfies(idx_t k)
{
    idx_t n   = k + 2;
    idx_t tmp = n;
    /* Factor n using primes up to sqrt(n). */
    int need = (int)sqrt((double)n) + 2;
    extend_primes(need);

    for (size_t i = 0; i < prime_cnt && (idx_t)prime_list[i] * (idx_t)prime_list[i] <= tmp; ++i) {
        int p = prime_list[i];
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (!satisfies_prime(k, (idx_t)p, exp)) return false;
        }
    }
    if (tmp > 1) {               /* remaining prime factor */
        if (!satisfies_prime(k, tmp, 1)) return false;
    }
    return true;
}

/* Ensure seq_terms has capacity for at least 'min_cap' elements. */
static void ensure_terms_capacity(idx_t min_cap)
{
    if (seq_cap >= min_cap) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < min_cap) new_cap *= 2;
    seq_terms = (seq_t*)realloc(seq_terms, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Generate terms until we have at least 'need' terms. */
static void generate_up_to(idx_t need)
{
    if (seq_len >= need) return;
    if (!prime_list) { extend_primes(100); }   /* initial primes */

    while (seq_len < need) {
        if (k_satisfies(next_k)) {
            ensure_terms_capacity(seq_len + 1);
            seq_terms[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

/* Public routine. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n < 1 */
    generate_up_to(n);
    return seq_terms[n - 1];
}
