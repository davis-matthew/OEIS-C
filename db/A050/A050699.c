/*
 * A050699  Nonprime numbers n such that n and n-reversed (<> n and no leading
 *          zeros) have the same number of prime factors (counted with
 *          multiplicity).
 * https://oeis.org/A050699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nonprime numbers n (n ≥ 10, not a palindrome, not ending with 0) such that
   n and its decimal reverse have the same number of prime factors
   (counted with multiplicity).  Both n and its reverse must be
   represented without leading zeros.  The function builds the list on
   demand and memoises already-seen terms. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int *prime_list = NULL;
static size_t prime_count = 0;
static bool primes_inited = false;

/* Build a small primality sieve up to LIMIT.  LIMIT 2 000 000 covers
   primes up to 2 million which is enough for all integers examined up
   to the million‑th term (far beyond the 10 000‑th term). */
static void init_primes(void)
{
    if (primes_inited) return;
    const int LIMIT = 2000000;
    char *sieve = (char *)malloc(LIMIT + 1);
    if (!sieve) abort();
    memset(sieve, 1, LIMIT + 1);
    sieve[0] = sieve[1] = 0;
    for (int i = 2; (long long)i * i <= LIMIT; ++i)
        if (sieve[i])
            for (int j = i * i; j <= LIMIT; j += i)
                sieve[j] = 0;
    prime_count = 0;
    for (int i = 2; i <= LIMIT; ++i)
        if (sieve[i]) prime_count++;
    prime_list = (int *)malloc(prime_count * sizeof(int));
    if (!prime_list) abort();
    size_t idx = 0;
    for (int i = 2; i <= LIMIT; ++i)
        if (sieve[i]) prime_list[idx++] = i;
    free(sieve);
    primes_inited = true;
}

/* Count prime factors (with multiplicity) of n using prime_list. */
static unsigned int count_pf(uint64_t n)
{
    if (n < 2) return 0;
    unsigned int cnt = 0;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= n; ++i) {
        int p = prime_list[i];
        while (n % p == 0) {
            cnt++;
            n /= p;
        }
    }
    if (n > 1) cnt++;          /* remaining prime factor */
    return cnt;
}

/* Simple primality test using prime_list. */
static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= n; ++i) {
        int p = prime_list[i];
        if (n % p == 0) return false;
    }
    return true;
}

/* Reverse the decimal digits of n. */
static uint64_t rev_uint64(uint64_t n)
{
    uint64_t rev = 0;
    while (n > 0) {
        rev = rev * 10 + n % 10;
        n /= 10;
    }
    return rev;
}

/* Return the n‑th term of sequence A050699.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t next_cand = 10;          /* start checking from 10 */

    init_primes();

    while (terms_len < n) {
        uint64_t cand;
        for (;;) {
            cand = next_cand++;
            if (cand % 10 == 0) continue;          /* skip trailing 0 (would give leading 0 in reverse) */
            uint64_t rev = rev_uint64(cand);
            if (rev == cand) continue;            /* skip palindromes */
            if (is_prime(cand)) continue;          /* must be non‑prime */
            if (count_pf(cand) == count_pf(rev))
                break;                             /* found next term */
        }
        /* store found term */
        if (terms_len == terms_cap) {
            terms_cap = terms_cap ? terms_cap * 2 : 16;
            terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            if (!terms) abort();
        }
        terms[terms_len++] = (seq_t)cand;
    }
    return terms[n - 1];
}
