/*
 * A094308  Row sums of A094307.
 * https://oeis.org/A094308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(1,2,…,n) * ( n - Σ_{p≤n, p^k > n/2} (1 - 1/p) )
 *            Equivalently a(n) = L * n - Σ_{p≤n, p^k > n/2} (L/p) * (p-1)
 *            where L = lcm(1,2,…,n) and the sum runs over primes p whose
 *            greatest power ≤ n is larger than n/2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses the utility function  
 *  build_primes_upto(idx_t limit, uint64_t **primes, size_t *cnt)   
 *  which returns the list of all primes ≤ limit and their count.  */
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Cache primes up to the largest n seen so far. */
    static idx_t  max_limit = 0;
    static uint64_t *primes = NULL;
    static size_t   prime_cnt = 0;

    if (n > max_limit) {
        if (primes) free(primes);
        build_primes_upto(n, &primes, &prime_cnt);
        max_limit = n;
    }

    /* First compute L = lcm(1..n) = ∏ p^⌊log_p(n)⌋. */
    seq_t L = (seq_t)1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t pe = (seq_t)1;
        seq_t limit = (seq_t)n;
        while (pe <= limit / (seq_t)p) {
            pe *= (seq_t)p;
        }
        L *= pe;
    }

    /* Sum over primes with p^k > n/2.  */
    seq_t sum_terms = (seq_t)0;
    seq_t half = (seq_t)n / 2;   /* floor(n/2) */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        /* compute the largest power of p that does not exceed n */
        seq_t pe = (seq_t)1;
        seq_t limit = (seq_t)n;
        while (pe <= limit / (seq_t)p) {
            pe *= (seq_t)p;
        }
        if (pe > half) {                   /* condition p^k > n/2 */
            seq_t L_div_p = L / (seq_t)p;   /* L is divisible by p */
            seq_t term    = L_div_p * (seq_t)(p - 1);
            sum_terms += term;
        }
    }

    /* a(n) = L*n - sum_terms */
    seq_t result = L * (seq_t)n - sum_terms;
    return result;
}
