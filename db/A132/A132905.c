/*
 * A132905  Numbers formed by concatenating 5 consecutive prime numbers.
 * https://oeis.org/A132905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concat(primes[n], primes[n+1], primes[n+2], primes[n+3],
 *            primes[n+4])
 * Range:     u8 none, u16 none, u32 n <= 13, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   build_primes_upto, digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenates five consecutive primes starting with the nth prime (n ≥ 1). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow10.h"
#include "../utils/digit_count.h"

static uint64_t *prime_tbl = NULL;
static size_t prime_cnt   = 0;
static idx_t prime_limit   = 0;

static void init_primes(idx_t n)
{
    /* Allocate primes if not already done. */
    if (prime_tbl == NULL) {
        prime_limit = 200000;  /* initial bound */
        build_primes_upto(prime_limit, &prime_tbl, &prime_cnt);
    }
    /* Extend prime list until we have at least n+4 primes. */
    while ((idx_t)prime_cnt < n + 4) {
        prime_limit <<= 1;
        free(prime_tbl);
        build_primes_upto(prime_limit, &prime_tbl, &prime_cnt);
    }
}

seq_t a(idx_t n)
{
    init_primes(n);

    size_t start = (size_t)(n - 1); /* primes[0] is the 1st prime */
    seq_t res = 0;

    for (idx_t i = 0; i < 5; ++i) {
        uint64_t p = prime_tbl[start + (size_t)i];
        idx_t  digs = digit_count(p);
        seq_t  p10  = pow10(digs);
        res = res * p10 + (seq_t)p;
    }
    return res;
}
