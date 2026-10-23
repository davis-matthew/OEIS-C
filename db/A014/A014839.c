/*
 * A014839  Sum of all the digits of n in every prime-power base from 2 to
 *          n-1.
 * https://oeis.org/A014839
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A014839: a(n) = sum_{b prime power, 2<=b<=n-1} digit_sum(n in base b) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Simple sieve: return all primes <= limit.  Caller frees the array.
   limit is an idx_t (uint64_t). */
static idx_t *sieve_primes(idx_t limit, size_t *cnt)
{
    if (limit < 2) {
        *cnt = 0;
        return NULL;
    }

    /* allocate bool flag array (unsigned char) */
    unsigned char *composite = (unsigned char *)calloc((size_t)(limit + 1), sizeof(unsigned char));
    if (!composite)
        return NULL;

    size_t prim_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!composite[(size_t)i]) {
            prim_cnt++;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    composite[(size_t)j] = 1;
            }
        }
    }

    idx_t *primes = (idx_t *)malloc(prim_cnt * sizeof(idx_t));
    if (!primes) {
        free(composite);
        *cnt = 0;
        return NULL;
    }

    size_t pidx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!composite[(size_t)i])
            primes[pidx++] = i;
    }

    free(composite);
    *cnt = prim_cnt;
    return primes;
}

/* Compute the sum of digits of value expressed in base 'base'. */
static seq_t digit_sum(seq_t value, seq_t base)
{
    seq_t sum = 0;
    seq_t tmp  = value;
    while (tmp > 0) {
        sum += tmp % base;
        tmp  /= base;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                /* undefined for n<3; return 0 */

    idx_t limit = n - 1;
    size_t pcount;
    idx_t *primes = sieve_primes(limit, &pcount);
    if (!primes)
        return 0;                /* allocation failed */

    seq_t total = 0;
    seq_t nseq  = (seq_t)n;

    for (size_t pi = 0; pi < pcount; ++pi) {
        idx_t p = primes[pi];
        seq_t base = (seq_t)p;
        seq_t power = base;

        while (power <= (seq_t)limit) {
            total += digit_sum(nseq, power);

            /* next power: check for overflow before multiplication */
            if (power > (seq_t)limit / base)
                break;
            power *= base;
        }
    }

    free(primes);
    return total;
}
