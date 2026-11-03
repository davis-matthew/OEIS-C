/*
 * A071225  n-th power of the product of next n primes.
 * https://oeis.org/A071225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{k=1}^{n} p_{T_{n-1}+k})^n, where T_m = m(m+1)/2
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_idx.h"

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static uint64_t *prime_cache = NULL;
static size_t prime_cache_count = 0;

/* Ensure that prime_cache contains at least 'need' primes (1‑based count). */
static void ensure_primes_count(idx_t need)
{
    if (prime_cache_count >= (size_t)need)
        return;

    /* Upper bound on the nth prime. */
    idx_t limit = prime_limit_for_idx(need);

    uint64_t *tmp = NULL;
    size_t tmp_cnt = 0;
    build_primes_upto(limit, &tmp, &tmp_cnt);

    /* The helper guarantees tmp_cnt >= need; no further check is needed. */
    /* Replace the old cache. */
    if (prime_cache)
        free(prime_cache);
    prime_cache = tmp;
    prime_cache_count = tmp_cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* T_{n-1} = (n-1)*n/2  (0‑based start index of product). */
    idx_t start_index = (n - 1) * n / 2;   /* inclusive */
    idx_t end_index   = start_index + n;   /* exclusive */

    /* Ensure we have enough primes. */
    ensure_primes_count(end_index);

    seq_t prod = 1;
    for (idx_t i = start_index; i < end_index; ++i)
        prod = seq_mul(prod, (seq_t)prime_cache[i]);

    return ipow_idx(prod, n);
}
