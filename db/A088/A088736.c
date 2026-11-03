/*
 * A088736  10^p - p for prime p.
 * https://oeis.org/A088736
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^p - p where p is the (n‑1)-th prime
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 12 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

static uint64_t *prime_buf = NULL;   /* stored primes, ascending */
static size_t prime_cnt = 0;         /* number of primes stored */
static idx_t prime_limit = 0;        /* highest prime that can be represented in prime_buf */

/* Return 10^p - p for the (n‑1)-th prime (n >= 2).           */
seq_t a(idx_t n)
{
    /* a(n) is defined only for n >= 2; lower indices return 0. */
    if (n < 2) return 0;

    /* Determine which prime we need: 1‑based index.          */
    idx_t target_index = n - 1;            /* 1‑based                           */

    /* Ensure we have enough primes in prime_buf.            */
    idx_t needed_limit = _prime_limit_for_idx(target_index);
    if (needed_limit > prime_limit) {
        /* Build a new table up to needed_limit.             */
        uint64_t *new_buf = NULL;
        size_t new_cnt = 0;
        build_primes_upto(needed_limit, &new_buf, &new_cnt);

        /* Replace old buffer.                                 */
        if (prime_buf) free(prime_buf);
        prime_buf = new_buf;
        prime_cnt = new_cnt;
        prime_limit = needed_limit;
    }

    /* Access the prime: target_index is 1‑based, array is 0‑based. */
    uint64_t p = prime_buf[(size_t)(target_index - 1)];

    /* Compute 10^p - p.  pow10 is provided in utils.          */
    seq_t val = pow10(p);
    val -= (seq_t)p;     /* cast to seq_t for subtraction */

    return val;
}
