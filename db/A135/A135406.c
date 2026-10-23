/*
 * A135406  Sum of squares of gaps between consecutive semiprimes.
 * https://oeis.org/A135406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A135406: Sum of squares of gaps between consecutive semiprimes
   a(n) = Σ_{k=1..n} (gap_k)^2 = Σ_{k=1..n} (A001358(k+1)-A001358(k))^2
   (using 1‑based indexing for the sequence). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- global state -------------------------------------------------- */

/* list of semiprimes found so far */
static seq_t *semiprimes = NULL;
static size_t semiprime_cnt = 0;     /* actual number stored */
static size_t semiprime_cap = 0;     /* allocated capacity */

/* cumulative sums of squared gaps; cumulative[i] = a(i+1) */
static seq_t *cumulative = NULL;
static size_t cum_cnt = 0;           /* number of filled entries */
static size_t cum_cap = 0;           /* allocated capacity */

/* small prime table for factorisation */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;
static uint64_t primes_limit = 1;    /* primes up to this limit are known */

/* next integer to test for semiprime */
static uint64_t next_candidate = 4;

/* ---------- helper functions --------------------------------------------- */

/* Extend the prime table so that all primes ≤ limit are known. */
static void extend_primes_to(uint64_t limit)
{
    if (limit <= primes_limit)
        return;

    uint64_t new_limit = limit + 100;         /* buffer to reduce rebuilds */
    bool *is_prime = (bool *)malloc((size_t)(new_limit + 1) * sizeof(bool));
    if (!is_prime) exit(1);

    for (size_t i = 0; i <= new_limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (uint64_t i = 2; i * i <= new_limit; ++i) {
        if (is_prime[(size_t)i]) {
            for (uint64_t j = i * i; j <= new_limit; j += i)
                is_prime[(size_t)j] = false;
        }
    }

    size_t new_cap = new_limit / 2;           /* over‑estimate */
    if (primes_cap < new_cap) {
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        if (!tmp) exit(1);
        primes = tmp;
        primes_cap = new_cap;
    }

    primes_cnt = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (is_prime[(size_t)i])
            primes[primes_cnt++] = (uint32_t)i;

    free(is_prime);
    primes_limit = new_limit;
}

/* Test whether x is a semiprime (product of two primes, multiplicities counted). */
static bool is_semiprime(uint64_t x)
{
    size_t cnt = 0;
    uint64_t tmp = x;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > tmp)
            break;
        while (tmp % p == 0) {
            ++cnt;
            tmp /= p;
            if (cnt > 2)
                return false;
        }
    }
    if (tmp > 1)          /* remaining prime factor */
        ++cnt;

    return cnt == 2;
}

/* Grow the semiprime array and cumulative array until at least 'required'
   semiprimes have been stored.  While doing so, the cumulative sums are
   computed. */
static void ensure_semiprimes(idx_t required)
{
    if (semiprime_cnt >= (size_t)required)
        return;

    /* enlarge semiprime array */
    if (semiprime_cap < (size_t)required) {
        size_t cap = semiprime_cap ? semiprime_cap * 2 : 128;
        if (cap < (size_t)required)
            cap = (size_t)required;
        seq_t *tmp = (seq_t *)realloc(semiprimes, cap * sizeof(seq_t));
        if (!tmp) exit(1);
        semiprimes = tmp;
        semiprime_cap = cap;
    }

    /* enlarge cumulative array; it stores (semiprime_cnt - 1) entries */
    if (cum_cap < (required - 1)) {
        size_t cap = cum_cap ? cum_cap * 2 : 128;
        if (cap < (required - 1))
            cap = (required - 1);
        seq_t *tmp = (seq_t *)realloc(cumulative, cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cumulative = tmp;
        cum_cap = cap;
    }

    /* generate semiprimes */
    while (semiprime_cnt < (size_t)required) {
        if (next_candidate > primes_limit)
            extend_primes_to(next_candidate);

        if (is_semiprime(next_candidate)) {
            semiprimes[semiprime_cnt++] = (seq_t)next_candidate;

            if (semiprime_cnt > 1) {
                seq_t gap = (seq_t)next_candidate - semiprimes[semiprime_cnt - 2];
                seq_t sq = gap * gap;
                seq_t sum = sq;
                if (cum_cnt > 0)
                    sum = cumulative[cum_cnt - 1] + sq;
                cumulative[cum_cnt++] = sum;
            }
        }
        ++next_candidate;
    }
}

/* ---------- public sequence ---------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Need n gaps → n+1 semiprimes */
    ensure_semiprimes((idx_t)n + 1);

    /* cumulative is zero‑based: cumulative[i] = a(i+1) */
    return cumulative[(size_t)n - 1];
}
