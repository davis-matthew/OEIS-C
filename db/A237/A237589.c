/*
 * A237589  Sum of first n odd noncomposite numbers.
 * https://oeis.org/A237589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + sum of the first n-1 odd primes (i.e. a(1)=1,
 *            a(2)=1+3=4, a(3)=1+3+5=9, …)
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state shared across calls */
    static size_t primes_cnt = 0;              /* number of primes known */
    static size_t capacity   = 0;              /* allocated size of primes / sum arrays */
    static seq_t *primes     = NULL;           /* primes[0..] holds odd primes */
    static seq_t *cum_sum    = NULL;           /* cum_sum[i] = sum of first i primes, cum_sum[0] = 0 */
    static uint64_t candidate = 3;             /* next odd number to test for primality */
    static bool init = false;

    if (!init) {
        capacity = 8;                            /* initial allocation size */
        primes   = (seq_t *)malloc(capacity * sizeof(seq_t));
        cum_sum  = (seq_t *)malloc((capacity + 1) * sizeof(seq_t));
        if (!primes || !cum_sum) exit(1);        /* allocation failure */
        cum_sum[0] = (seq_t)0;
        primes_cnt = 0;
        init = true;
    }

    if (n == 0) return (seq_t)0;                /* safety, not defined by OEIS */

    /* We need the sum of the first n-1 odd primes.
       So generate primes until we have at least n-1 of them. */
    idx_t needed = (n > 1) ? n - 1 : 0;
    while ((idx_t)primes_cnt < needed) {
        /* Find the next prime candidate. */
        while (1) {
            bool is_prime = true;
            /* test divisibility by known primes up to sqrt(candidate) */
            for (size_t i = 0; i < primes_cnt; ++i) {
                seq_t p = primes[i];
                if (p * p > (seq_t)candidate) break;
                if ((uint64_t)candidate % (uint64_t)p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) break;               /* candidate is prime */
            candidate += 2;                    /* try next odd number */
        }

        /* Ensure capacity for the new prime and cumulative sum. */
        if (primes_cnt + 1 > capacity) {
            size_t new_cap = capacity << 1;    /* double the capacity */
            seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            primes = tmp;
            tmp = (seq_t *)realloc(cum_sum, (new_cap + 1) * sizeof(seq_t));
            if (!tmp) exit(1);
            cum_sum = tmp;
            capacity = new_cap;
        }

        /* Store the newly found prime and update cumulative sum. */
        primes[primes_cnt] = (seq_t)candidate;
        cum_sum[primes_cnt + 1] = cum_sum[primes_cnt] + (seq_t)candidate;
        ++primes_cnt;
        candidate += 2;                        /* advance to the next odd candidate */
    }

    /* Compute a(n) = 1 + sum of the first n-1 odd primes. */
    seq_t result = cum_sum[needed] + (seq_t)1;
    return result;
}
