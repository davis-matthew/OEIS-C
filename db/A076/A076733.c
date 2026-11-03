/*
 * A076733  Largest k such that k! divides C(2n,n).
 * https://oeis.org/A076733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum k such that k! divides C(2n,n) (central binomial
 *            coefficient)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9693 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* exponent of prime p in C(2n,n) */
static idx_t exp_binom(idx_t n, idx_t p)
{
    idx_t e  = 0;
    idx_t x  = 2 * n;
    while (x) {
        x /= p;
        e += x;
    }
    x = n;
    while (x) {
        x /= p;
        e -= 2 * x;
    }
    return e;
}

/* Legendre function: exponent of `p` in `n!`. */
static inline idx_t exp_fact(idx_t n, idx_t p) {
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* Prime sieve caching: primes up to the current limit   */
static uint64_t *prime_list  = NULL;   /* array of primes (uint64_t)   */
static size_t    prime_cnt   = 0;      /* number of primes stored     */
static idx_t     prime_limit = 0;      /* largest number that was sieved */

static void prime_sieve(idx_t limit)
{
    if (prime_limit >= limit) return;        /* already large enough */

    /* Re‑create sieve from scratch */
    if (prime_list != NULL) {
        free(prime_list);
        prime_list = NULL;
        prime_cnt  = 0;
        prime_limit = 0;
    }

    /* Simple sieve of Eratosthenes */
    /* Allocate a boolean array for composites */
    size_t sz = (size_t)limit + 1;
    bool *comp = (bool *)malloc(sz);
    if (!comp) return;                      /* allocation failure */

    for (size_t i = 0; i < sz; ++i) comp[i] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!comp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                comp[j] = true;
        }
    }

    /* Count primes first */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!comp[i]) ++cnt;

    uint64_t *list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!list) { free(comp); return; }

    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!comp[i]) list[pos++] = (uint64_t)i;

    /* Store results */
    prime_list  = list;
    prime_cnt   = cnt;
    prime_limit = limit;

    free(comp);
}

/* exponent of prime p in n! */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined but harmless */

    /* Ensure we have all primes up to 2n */
    prime_sieve(2 * n);
    if (prime_list == NULL || prime_cnt == 0) return 0;  /* allocation failure */

    /* Pre‑compute exponents of each prime in C(2n,n) */
    idx_t *e_binom = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    if (!e_binom) return 0;                    /* allocation failure */

    for (size_t i = 0; i < prime_cnt; ++i)
        e_binom[i] = exp_binom(n, prime_list[i]);

    /* Binary search for the largest k with k! | C(2n,n) */
    idx_t low  = 1;
    idx_t high = 2 * n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        bool ok = true;

        /* check all primes <= mid */
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= mid; ++i) {
            if (exp_fact(mid, prime_list[i]) > e_binom[i]) {
                ok = false;
                break;
            }
        }

        if (ok) low  = mid;
        else    high = mid - 1;
    }

    free(e_binom);
    return (seq_t)low;
}
