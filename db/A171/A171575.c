/*
 * A171575  n-th noncomposite number plus n-th even nonprime number.
 * https://oeis.org/A171575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008578(n) + A163300(n) A008578(n) = 1 (n = 1) or n-th
 *            prime (n > 1) A163300(n) = 0 (n = 1) or 2*n (n > 1)
 * Range:     u8 n <= 41, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* --- prime cache ----------------------------------------------------- */

static idx_t   *prime_cache  = NULL;   /* array of primes (uint64_t)   */
static size_t  prime_cap    = 0;       /* capacity of the array       */
static size_t  prime_cnt    = 0;       /* number of primes stored     */
static idx_t   next_cand    = 2;       /* next integer to test        */

/* Expand the prime cache until it contains at least 'need' primes
   (need ≥ 0).  Uses a simple incremental trial‑division generator. */
static void
ensure_primes(idx_t need)
{
    if (need <= (idx_t)prime_cnt)          /* already have enough   */
        return;

    /* Ensure the array has enough capacity. */
    if (prime_cap < prime_cnt + 8) {
        size_t newcap = prime_cap ? prime_cap * 2 : 16;
        if (newcap < prime_cnt + 8)
            newcap = prime_cnt + 8;
        idx_t *tmp = (idx_t *)realloc(prime_cache,
                                      newcap * sizeof(idx_t));
        if (!tmp) {
            /* Out of memory – terminate program. */
            exit(EXIT_FAILURE);
        }
        prime_cache = tmp;
        prime_cap   = newcap;
    }

    /* Generate primes until the required amount is reached. */
    while (prime_cnt < need) {
        idx_t cand = next_cand;
        int is_pr = 1;
        for (size_t i = 0; i < prime_cnt && prime_cache[i] * prime_cache[i] <= cand; ++i) {
            if (cand % prime_cache[i] == 0) {
                is_pr = 0;
                break;
            }
        }
        if (is_pr) {
            prime_cache[prime_cnt++] = cand;
        }
        ++next_cand;
    }
}

/* --- sequence function ------------------------------------------------- */

seq_t
a(idx_t n)
{
    if (n == 0)            /* undefined for the OEIS offset 1   */
        return (seq_t)0;

    /* A008578 part */
    seq_t prime_term;
    if (n == 1) {
        prime_term = (seq_t)1;          /* 1 is the first noncomposite */
    } else {
        /* nth noncomposite for n>1 is the (n-1)‑th prime. */
        ensure_primes((idx_t)(n - 1));
        prime_term = (seq_t)prime_cache[(size_t)(n - 2)];
    }

    /* A163300 part */
    seq_t even_nonprime;
    if (n == 1) {
        even_nonprime = (seq_t)0;        /* the first even nonprime      */
    } else {
        even_nonprime = (seq_t)2 * (seq_t)n;
    }

    return prime_term + even_nonprime;
}
