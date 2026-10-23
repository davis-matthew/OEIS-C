/*
 * A171140  Numbers k such that 6*k + 7 = p^2 (p=prime).
 * https://oeis.org/A171140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_{n+2}^2 - 7) / 6 where p_m is the m‑th prime (p_1 = 2)
 * Range:     u8 n <= 10, u16 n <= 112, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This returns the n‑th element of OEIS A171140 (offset 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;   /* list of primes returned by build_primes_upto */
static size_t   prime_count = 0;      /* number of primes currently stored */

/* Ensure that prime_list contains at least 'need' primes (1‑based count). */
static void
ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;                                   /* already enough */

    /* Upper bound for the n-th prime: for n >= 6, p_n < n(ln n + ln ln n).
       For safety, we use a multiplier that works for n up to 10^5.          */
    idx_t limit = (idx_t)(need * 15 + 100);

    uint64_t *tmp_primes = NULL;
    size_t    tmp_cnt    = 0;
    build_primes_upto(limit, &tmp_primes, &tmp_cnt);

    /* Replace any previous allocation. */
    free(prime_list);
    prime_list = tmp_primes;
    prime_count = tmp_cnt;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                     /* behaviour for n < 1 is irrelevant */
        return (seq_t)0;

    /* p_{n+2} is stored at index (n+1) 0‑based (primes 2,3,5,7,...) */
    ensure_primes((size_t)n + 2);

    uint64_t p = prime_list[(size_t)n + 1];     /* the (n+2)-th prime */
    uint64_t k = (p * p - 7ULL) / 6ULL;         /* (p² - 7)/6 is always integer */

    return (seq_t)k;                /* cast to the build's seq_t type */
}
