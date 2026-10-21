/*
 * A076100  Least common multiple of n numbers starting with n.
 * https://oeis.org/A076100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, n+1, …, 2n‑1) (least common multiple of n numbers
 *            starting with n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Simple Sieve of Eratosthenes:  generate all primes ≤ limit. */
static void sieve_primes(idx_t limit, idx_t **primes, size_t *cnt)
{
    *primes = NULL;
    *cnt = 0;
    if (limit < 2) return;

    /* Boolean array of compositeness */
    unsigned char *is_comp = (unsigned char *)calloc(limit + 1, 1);
    size_t est = limit / 10 + 3;
    idx_t *p = (idx_t *)malloc(est * sizeof(idx_t));
    size_t pc = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (pc == est) {         /* enlarge buffer */
                est *= 2;
                p = (idx_t *)realloc(p, est * sizeof(idx_t));
            }
            p[pc++] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    *primes = p;
    *cnt = pc;
    free(is_comp);
}

/* Compute a(n).  For any n ≥ 1.  Returns 0 for n = 0. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    idx_t limit = 2 * n - 1;      /* upper bound of the range */

    /* --- generate primes up to 2n-1 --- */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    sieve_primes(limit, &primes, &prime_cnt);

    seq_t result = 1;

    /* --- for each prime, find the highest power that appears in the range --- */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        idx_t e = 0;
        idx_t pow = 1;

        while (1) {
            idx_t newpow = pow * p;
            if (newpow > limit) break;

            idx_t low  = (n + newpow - 1) / newpow;   /* ceil(n / newpow) */
            idx_t high = limit / newpow;              /* floor(limit / newpow) */

            if (low <= high) {          /* at least one multiple in the range */
                pow = newpow;
                ++e;
            } else {
                break;
            }
        }

        if (e > 0) {
            seq_t powseq = 1;
            seq_t pseq   = (seq_t)p;
            for (idx_t j = 0; j < e; ++j)
                powseq = powseq * pseq;
            result = result * powseq;
        }
    }

    free(primes);
    return result;
}
