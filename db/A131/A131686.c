/*
 * A131686  Sum of squares of five consecutive primes.
 * https://oeis.org/A131686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n^2 + p_{n+1}^2 + p_{n+2}^2 + p_{n+3}^2 + p_{n+4}^2,
 *            where p_k is the k‑th prime (p_1 = 2)
 * Range:     u8 n <= 1, u16 n <= 27, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper build_primes_upto from ../utils/build_primes_upto.h */

#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_arr = NULL;   /* array of primes (p_1 ... p_m) */
static size_t     prime_cnt = 0;     /* number of primes stored */
static idx_t      sieve_limit = 0;   /* highest value used in sieve */

/* Estimate an upper bound for the m‑th prime.
   For m ≥ 6, p_m ≤ m (log m + log log m) + 3.  */
static idx_t estimate_limit(idx_t m)
{
    if (m < 6) return 15;  /* works for small m  */
    double dm = (double)m;
    double est = dm * (log(dm) + log(log(dm))) + 3.0;
    return (idx_t)(est) + 10;
}

/* Ensure we have at least n+4 primes in prime_arr.  */
static void ensure_primes(idx_t n)
{
    idx_t need_index = n + 4;      /* need primes[0 .. need_index-1] */
    if ((idx_t)prime_cnt >= need_index) return;

    idx_t limit = estimate_limit(need_index);
    while (true) {
        size_t cnt = 0;
        uint64_t *tmp = NULL;
        build_primes_upto(limit, &tmp, &cnt);
        if (cnt >= need_index) {
            if (prime_arr) free(prime_arr);
            prime_arr = tmp;
            prime_cnt = cnt;
            sieve_limit = limit;
            break;
        }
        free(tmp);
        limit *= 2;          /* double the limit and try again */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset 1; undefined for n < 1 */
    ensure_primes(n);

    seq_t sum = 0;
    size_t idx_start = (size_t)(n - 1);   /* convert to 0‑based index */
    for (size_t i = 0; i < 5; ++i) {
        seq_t p = (seq_t)prime_arr[idx_start + i];
        sum += p * p;
    }
    return sum;
}
