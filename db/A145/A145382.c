/*
 * A145382  Write the n-th prime in binary. Change all 0's to 1's and all 1's
 *          to 0's. a(n) is the decimal equivalent of the result.
 * https://oeis.org/A145382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 2^{1+floor(log2(p(n)))} - p(n) - 1
 * Range:     u8 n <= 97, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 84 terms through n = 971 (2026-11-01)
 * Helpers:   build_primes_upto, ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is obtained by inverting the binary representation of the n‑th prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ilog2.h"

/* utility – estimate an upper bound for the n-th prime (Rosser–Schoenfeld) */
static idx_t prime_limit_for_idx(idx_t n)
{
    if (n < 6)
        return 12;                               /* prime(6)=13 */
    double dn   = (double)n;
    double lgn  = log(dn);
    double lgln = log(lgn);
    /* p_n < n(ln n + ln ln n) for n ≥ 6, add a safety margin */
    double limit = dn * (lgn + lgln) + 3.0;
    return (idx_t)limit;
}

seq_t a(idx_t n)
{
    static uint64_t *primes = NULL;  /* array of primes up to the current limit */
    static size_t   prime_cnt = 0;   /* number of primes stored */
    /* ensure we have at least n primes */
    if ((size_t)n > prime_cnt) {
        idx_t new_limit = prime_limit_for_idx(n);
        uint64_t *tmp_primes = NULL;
        size_t tmp_cnt = 0;
        build_primes_upto(new_limit, &tmp_primes, &tmp_cnt);

        /* free old data and replace */
        free(primes);
        primes    = tmp_primes;
        prime_cnt = tmp_cnt;
    }

    /* fetch the n‑th prime (1‑based indexing) */
    uint64_t p = primes[(size_t)n - 1];

    /* compute 2^{1+floor(log2(p))} */
    int msb = ilog2(p);
    seq_t top = ((seq_t)1) << (msb + 1);

    /* answer = top - p - 1 */
    seq_t answer = top - (seq_t)p - (seq_t)1;
    return answer;
}
