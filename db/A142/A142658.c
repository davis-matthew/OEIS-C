/*
 * A142658  Primes congruent to 41 mod 56.
 * https://oeis.org/A142658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p with p % 56 == 41 (OEIS A142658)
 * Range:     u8 n <= 2, u16 n <= 273, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- utilities for dynamic uint64_t array --- */
static inline void grow_array(uint64_t **arr, size_t *cnt, size_t *cap, size_t new_cnt)
{
    if (*cnt >= *cap)
    {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        while (new_cap < new_cnt) new_cap <<= 1;
        *arr = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        *cap = new_cap;
    }
}

/* --- primality test using previously computed small primes --- */
static bool is_prime_candidate(uint64_t n, uint64_t *primes, size_t primes_cnt)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= n; ++i)
    {
        if (n % primes[i] == 0) return false;
    }
    return true;
}

/* --- grow the list of small primes up to 'limit' --- */
static void ensure_primes_upto(uint64_t limit,
                               uint64_t **primes, size_t *cnt, size_t *cap,
                               uint64_t *prime_curr)
{
    /* start at 2 if first time */
    if (*prime_curr == 0) *prime_curr = 2;
    while (*prime_curr <= limit)
    {
        bool is_p = true;
        for (size_t i = 0; i < *cnt && (*primes)[i] * (*primes)[i] <= *prime_curr; ++i)
        {
            if (*prime_curr % (*primes)[i] == 0) { is_p = false; break; }
        }
        if (is_p)
        {
            grow_array(primes, cnt, cap, *cnt + 1);
            (*primes)[*cnt] = *prime_curr;
            ++(*cnt);
        }
        ++(*prime_curr);
    }
}

/* --- sequence function --- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* static cache of computed primes of the form 41 mod 56 */
    static uint64_t *target_primes = NULL;
    static size_t   target_cnt    = 0, target_cap    = 0;
    /* static cache of small primes used for trial division */
    static uint64_t *small_primes = NULL;
    static size_t   small_cnt     = 0, small_cap     = 0;
    static uint64_t prime_curr    = 0;
    /* current index of k such that candidate = 41 + 56*k */
    static idx_t   candidate_k   = 0;

    /* return cached value if available */
    if (target_cnt >= (size_t)n)
        return (seq_t)target_primes[n - 1];

    /* generate until we have at least n values */
    while (target_cnt < (size_t)n)
    {
        uint64_t cand = 41ULL + 56ULL * (uint64_t)candidate_k;

        /* ensure small primes up to sqrt(cand) are available */
        uint64_t sqrt_lim = (uint64_t)((double)cand * 0.5 + 1);  /* rough upper bound */
        while (sqrt_lim * sqrt_lim < cand) ++sqrt_lim;          /* tighten */
        ensure_primes_upto(sqrt_lim, &small_primes, &small_cnt, &small_cap, &prime_curr);

        if (is_prime_candidate(cand, small_primes, small_cnt))
        {
            grow_array(&target_primes, &target_cnt, &target_cap, target_cnt + 1);
            target_primes[target_cnt++] = cand;
        }
        ++candidate_k;
    }

    return (seq_t)target_primes[n - 1];
}
