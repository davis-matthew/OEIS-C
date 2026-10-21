/*
 * A066507  Numbers k such that there is a solution to x^2 == 2^k (mod k).
 * https://oeis.org/A066507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that x^2 ≡ 2^k (mod k) has a
 *            solution. Even k always satisfy the condition. For odd k this is
 *            true iff every prime factor p of k satisfies p ≡ ±1 (mod 8)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static int *prime_list = NULL;       /* array of computed primes */
static size_t prime_count = 0;       /* number of primes stored */
static idx_t prime_limit = 0;        /* highest n up to which primes are computed */

/* Build the list of primes up to n (inclusive). */
static void ensure_primes(idx_t n)
{
    if (n <= prime_limit) return;

    idx_t size = n + 1;
    bool *sieve = (bool *)malloc((size_t)size);
    if (!sieve) exit(1);

    for (idx_t i = 0; i < size; ++i) sieve[i] = true;
    if (size > 0) sieve[0] = false;
    if (size > 1) sieve[1] = false;

    for (idx_t i = 2; i * i <= n; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= n; j += i) sieve[j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i) if (sieve[i]) ++cnt;
    int *new_list = (int *)malloc(cnt * sizeof(int));
    if (!new_list) exit(1);
    size_t idx = 0;
    for (idx_t i = 2; i <= n; ++i) if (sieve[i]) new_list[idx++] = (int)i;

    free(prime_list);
    prime_list = new_list;
    prime_count = cnt;
    prime_limit = n;

    free(sieve);
}

/* Test whether k satisfies the property. */
static bool good_k(idx_t k)
{
    if ((k & 1) == 0) return true;     /* even k always good */

    /* k is odd */
    idx_t m = k;
    /* Ensure primes up to sqrt(k) */
    idx_t lim = k;
    for (idx_t p = 2; p * p <= lim; ++p) lim = p * p;
    ensure_primes(k);

    for (size_t i = 0; i < prime_count && (idx_t)prime_list[i] * prime_list[i] <= m; ++i) {
        int p = prime_list[i];
        if (m % p == 0) {
            if ((p & 7) != 1 && (p & 7) != 7) return false;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1 && ((m & 7) != 1 && (m & 7) != 7)) return false;
    return true;
}

static seq_t *terms = NULL;        /* stored terms so far */
static size_t terms_cap = 0;       /* allocated capacity */
static idx_t term_count = 0;       /* how many terms stored */
static idx_t next_k = 1;           /* next integer to test */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Grow the terms list until it contains at least n elements */
    while (term_count < n) {
        if (good_k(next_k)) {
            if (term_count >= terms_cap) {
                size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
                seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_terms) exit(1);
                terms = new_terms;
                terms_cap = new_cap;
            }
            terms[term_count++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return terms[n - 1];
}
