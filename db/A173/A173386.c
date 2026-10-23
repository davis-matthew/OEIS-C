/*
 * A173386  The absolute values of n-th natural noncomposite number minus n-th
 *          number k such that 6*k +- 1 is a twin prime pair.
 * https://oeis.org/A173386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = abs(A008578(n) - A002822(n))
 * Range:     u8 n <= 49, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*  Helper: test primality of 64‑bit integers. */
#include "../utils/is_prime.h"

/*   Dynamic array of primes (for A008578). */
static uint64_t  *prime_list   = NULL;
static size_t     prime_len    = 0;
static size_t     prime_cap    = 0;

/*   Dynamic array of twin‑prime indices (for A002822). */
static idx_t      *twink_list   = NULL;
static size_t     twink_len    = 0;
static size_t     twink_cap    = 0;
static idx_t      last_k       = 0;      /* last k processed for twin indices */

/*   Ensure space for at least 'need' primes. */
static void ensure_prime_cap(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = (prime_cap == 0) ? 16 : prime_cap << 1;
    while (new_cap < need) new_cap <<= 1;
    prime_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!prime_list) abort();
    prime_cap = new_cap;
}

/*   Ensure we have at least 'count' primes (count >= 1). */
static void ensure_prime_count_at_least(size_t count)
{
    if (prime_len >= count) return;

    uint64_t next_candidate = (prime_len == 0) ? 2ULL : prime_list[prime_len-1] + 1ULL;
    while (prime_len < count) {
        while (!is_prime(next_candidate)) ++next_candidate;
        ensure_prime_cap(prime_len + 1);
        prime_list[prime_len++] = next_candidate;
        ++next_candidate;
    }
}

/*   Ensure space for at least 'need' twin indices. */
static void ensure_twink_cap(size_t need)
{
    if (need <= twink_cap) return;
    size_t new_cap = (twink_cap == 0) ? 16 : twink_cap << 1;
    while (new_cap < need) new_cap <<= 1;
    twink_list = (idx_t *)realloc(twink_list, new_cap * sizeof(idx_t));
    if (!twink_list) abort();
    twink_cap = new_cap;
}

/*   Ensure we have at least 'count' twin indices (count >= 1). */
static void ensure_twink_count_at_least(size_t count)
{
    if (twink_len >= count) return;

    while (twink_len < count) {
        ++last_k;
        uint64_t a = 6ULL * last_k - 1ULL;
        uint64_t b = 6ULL * last_k + 1ULL;
        if (is_prime(a) && is_prime(b)) {
            ensure_twink_cap(twink_len + 1);
            twink_list[twink_len++] = last_k;
        }
    }
}

/*  Main sequence function.  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                /* undefined for n<1 */

    /*  A008578(n): 1 for n==1, otherwise the (n-1)th prime.  */
    seq_t noncomp;
    if (n == 1) {
        noncomp = (seq_t)1;
    } else {
        ensure_prime_count_at_least((size_t)(n - 1));
        noncomp = (seq_t)prime_list[(size_t)(n - 2)];
    }

    /*  A002822(n): nth twin‑prime index.  */
    ensure_twink_count_at_least((size_t)n);
    seq_t twin_k = (seq_t)twink_list[(size_t)(n - 1)];

    /*  Absolute difference.  */
    return (noncomp >= twin_k) ? (noncomp - twin_k) : (twin_k - noncomp);
}
