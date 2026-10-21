/*
 * A228355  Write the primes backwards in base 10 and juxtapose their digits.
 * https://oeis.org/A228355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   not stated
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: write primes backwards in base 10 and juxtapose
   their digits.  a(0)=2, a(1)=3, a(2)=5, a(3)=7, ...  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- internal helpers ---------- */
static seq_t *digit_cache = NULL;          /* concatenated reversed digits of primes */
static size_t digit_len = 0, digit_cap = 0;

static uint64_t *prime_list = NULL;       /* primes found so far */
static size_t prime_cnt = 0, prime_cap = 0;

static uint64_t candidate = 3;             /* next odd candidate (after 2) */

/* Ensure digit_cache has at least 'need' elements */
static void ensure_digit_cap(size_t need)
{
    if (need <= digit_cap) return;
    size_t new_cap = digit_cap ? digit_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(digit_cache, new_cap * sizeof(seq_t));
    digit_cache = tmp;
    digit_cap = new_cap;
}

/* Ensure prime_list has at least 'need' elements */
static void ensure_prime_cap(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    prime_list = tmp;
    prime_cap = new_cap;
}

/* Simple primality test using the primes found so far */
static bool is_candidate_prime(uint64_t num)
{
    if (num < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > num / p) break;          /* p² > num → stop */
        if (num % p == 0) return false;
    }
    return true;
}

/* Generate next prime, store it, and append its reversed digits */
static void generate_next_prime(void)
{
    /* Find the next prime */
    while (!is_candidate_prime(candidate))
        candidate += 2;                  /* skip evens */

    /* Store the prime */
    ensure_prime_cap(prime_cnt + 1);
    prime_list[prime_cnt++] = candidate;

    /* Append reversed digits of the prime */
    uint64_t tmp = candidate;
    size_t d = 0;
    if (tmp == 0) d = 1;
    while (tmp > 0) {
        ++d;
        tmp /= 10;
    }
    tmp = candidate;

    ensure_digit_cap(digit_len + d);
    for (size_t i = 0; i < d; ++i) {
        digit_cache[digit_len++] = (seq_t)(tmp % 10);
        tmp /= 10;
    }

    candidate += 2;                      /* next odd candidate */
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    /* Ensure the prime 2 and its digit 2 are present */
    if (prime_cnt == 0) {
        ensure_prime_cap(1);
        prime_list[0] = 2;
        prime_cnt = 1;

        ensure_digit_cap(digit_len + 1);
        digit_cache[digit_len++] = (seq_t)2;
    }

    /* Generate primes until we have at least n+1 digits */
    while (digit_len <= (size_t)n)
        generate_next_prime();

    return digit_cache[(size_t)n];
}
