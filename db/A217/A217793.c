/*
 * A217793  Erdős-Turán Golomb rulers, triangle read by rows.
 * https://oeis.org/A217793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = 2 * p * k + (k² mod p) with p = n‑th odd prime, 0 ≤ k <
 *            p. Sequence a(n) reads the triangle T by rows: a(1)=0, a(2)=7,
 *            a(3)=13, a(4)=0, …
 * Range:     u8 n <= 36, u16 n <= 3617, u32 n <= 7980, u64 n <= 7980,
 *            u128 n <= 7980, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* static state for prime generation and cumulative row lengths */
static idx_t  *primes      = NULL;   /* odd primes in order */
static idx_t  *cum_sum     = NULL;   /* cumulative sum of primes   */
static size_t prime_cnt    = 0;      /* number of primes stored    */
static size_t primes_cap   = 0;      /* allocated capacity         */

/* Ensure we have enough rows so that cum_sum[prime_cnt‑1] ≥ target. */
static void ensure_primes(idx_t target)
{
    /* First time: initialise with room for 1024 primes */
    if (prime_cnt == 0) {
        primes_cap = 1024;
        primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
        cum_sum = (idx_t *)malloc(primes_cap * sizeof(idx_t));
        /* first odd prime is 3 */
        primes[0] = 3;
        cum_sum[0] = 3;
        prime_cnt = 1;
    }

    /* Generate primes until the cumulative sum reaches or exceeds target */
    while (cum_sum[prime_cnt - 1] < target) {
        idx_t cand = primes[prime_cnt - 1] + 2;   /* skip even numbers */

        /* Find the next prime */
        while (true) {
            bool is_prime = true;
            for (size_t i = 0; i < prime_cnt &&
                    ((idx_t)primes[i] * primes[i] <= cand); ++i) {
                if (cand % primes[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                /* Grow the dynamic arrays if necessary */
                if (prime_cnt == primes_cap) {
                    primes_cap *= 2;
                    primes = (idx_t *)realloc(primes,
                                              primes_cap * sizeof(idx_t));
                    cum_sum = (idx_t *)realloc(cum_sum,
                                               primes_cap * sizeof(idx_t));
                }
                primes[prime_cnt] = cand;
                cum_sum[prime_cnt] = cum_sum[prime_cnt - 1] + cand;
                ++prime_cnt;
                break;   /* prime found, exit inner loop */
            }
            cand += 2;  /* try next odd candidate */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; a(0) is undefined, return 0 */
        return 0;

    ensure_primes(n);                     /* make sure enough rows exist */

    /* Locate the row containing the n‑th term */
    size_t row = 0;
    while (row < prime_cnt && n > cum_sum[row])  ++row;

    idx_t prev_sum = (row == 0) ? 0 : cum_sum[row - 1];
    idx_t k       = n - prev_sum - 1;          /* column index within the row */
    idx_t p       = primes[row];               /* the odd prime for this row */

    seq_t ps = (seq_t)p;
    seq_t ks = (seq_t)k;
    seq_t ksq_mod = (ks * ks) % ps;            /* k² mod p */
    seq_t result = (seq_t)(2 * ps * ks + ksq_mod);

    return result;
}
