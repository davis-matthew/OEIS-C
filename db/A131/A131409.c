/*
 * A131409  Middle of three consecutive semiprimes with common prime factor =
 *          3.
 * https://oeis.org/A131409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = middle of three consecutive semiprimes with common prime
 *            factor = 3
 * Range:     u8 none, u16 n <= 1, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Generate all primes <= limit, return array and count via cnt_out. */
static int* sieve_primes(int limit, int *cnt_out)
{
    char *isp = (char*)malloc((limit + 1) * sizeof(char));
    if (!isp) return NULL;
    for (int i = 0; i <= limit; ++i) isp[i] = 1;
    if (limit >= 0) isp[0] = 0;
    if (limit >= 1) isp[1] = 0;

    int sq = (int)sqrt((long double)limit);
    for (int i = 2; i <= sq; ++i) {
        if (isp[i]) {
            for (long long j = (long long)i * i; j <= limit; j += i) {
                isp[(int)j] = 0;
            }
        }
    }

    int *pr = (int*)malloc((limit + 1) * sizeof(int));
    if (!pr) {
        free(isp);
        return NULL;
    }
    int c = 0;
    for (int i = 2; i <= limit; ++i) if (isp[i]) pr[c++] = i;

    free(isp);
    *cnt_out = c;
    return pr;
}

/* Main routine */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* not defined for n<1 */

    const unsigned long long SEG_LEN  = 500000ULL;   /* segment size */
    unsigned long long low  = 2ULL;
    unsigned long long semSeen = 0;                /* semiprimes seen so far */
    unsigned long long triple = 0;                 /* triples found so far */

    /* ring buffer holding the last three semiprimes (newest first) */
    seq_t last0 = 0, last1 = 0, last2 = 0;

    /* allocate buffers once */
    static unsigned long long *residual = NULL;
    static unsigned char   *cnt      = NULL;
    if (!residual) {
        residual = (unsigned long long*)malloc(SEG_LEN * sizeof(unsigned long long));
        cnt      = (unsigned char*)  malloc(SEG_LEN * sizeof(unsigned char));
    }

    while (1) {
        unsigned long long high = low + SEG_LEN - 1;
        int limit = (int)sqrt((long double)high);
        if (limit < 2) limit = 2;

        int primeCnt = 0;
        int *primes  = sieve_primes(limit, &primeCnt);
        if (!primes) return (seq_t)0;    /* allocation failed */

        unsigned long long segLen = high - low + 1;
        if (segLen > SEG_LEN) segLen = SEG_LEN;   /* defensive */

        /* initialise buffers for this segment */
        for (unsigned long long i = 0; i < segLen; ++i) {
            residual[i] = low + i;
            cnt[i] = 0;
        }

        /* factorise every number in the segment by each prime */
        for (int pi = 0; pi < primeCnt; ++pi) {
            unsigned int p = (unsigned int)primes[pi];
            unsigned long long p2 = (unsigned long long)p * (unsigned long long)p;
            if (p2 > high) break;

            unsigned long long start = ((low + p - 1) / p) * (unsigned long long)p;
            for (unsigned long long j = start; j <= high; j += p) {
                unsigned long long idx = j - low;
                unsigned int mult = 0;
                while (residual[idx] % (unsigned long long)p == 0) {
                    residual[idx] /= (unsigned long long)p;
                    ++mult;
                }
                if (mult) cnt[idx] += mult;
            }
        }

        /* account for remaining prime factor > sqrt(high) */
        for (unsigned long long i = 0; i < segLen; ++i)
            if (residual[i] > 1) ++cnt[i];

        /* scan segment for semiprimes and detect triples */
        for (unsigned long long i = 0; i < segLen; ++i) {
            if (cnt[i] == 2) {
                seq_t num = (seq_t)(low + i);
                /* shift ring */
                last2 = last1;
                last1 = last0;
                last0 = num;
                ++semSeen;

                if (semSeen >= 3) {
                    if ((last2 % (seq_t)3) == 0 &&
                        (last1 % (seq_t)3) == 0 &&
                        (last0 % (seq_t)3) == 0) {
                        ++triple;
                        if (triple == (unsigned long long)n) {
                            free(primes);
                            return last1;   /* middle of the triple */
                        }
                    }
                }
            }
        }

        free(primes);
        low = high + 1;
    }
}
