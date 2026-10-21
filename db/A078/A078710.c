/*
 * A078710  Integer part of the subinterval sample variance in the partition
 *          of [0,n] by the divisors of n.
 * https://oeis.org/A078710
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (k*SS - n^2) / (k*(k-1)) ) where k is the number
 *            of divisors of n, SS is the sum of squares of the subinterval
 *            lengths defined by those divisors
 * Range:     u8 n <= 17, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Comparator for qsort of uint64_t */
static int cmp_uint64(const void *p, const void *q)
{
    uint64_t a = *(const uint64_t *)p;
    uint64_t b = *(const uint64_t *)q;
    return (a < b) ? -1 : (a > b);
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 2. For smaller indices
       we return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Enumerate all divisors of n (excluding 0). */
    size_t cap = 16;          /* initial capacity for divisor array */
    size_t cnt = 0;           /* number of divisors found */
    uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!divs)                 /* out of memory */
        return (seq_t)0;

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            if (cnt == cap) {
                cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
                if (!tmp) { free(divs); return (seq_t)0; }
                divs = tmp;
            }
            divs[cnt++] = d;
            if (d != n / d) {
                if (cnt == cap) {
                    cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
                    if (!tmp) { free(divs); return (seq_t)0; }
                    divs = tmp;
                }
                divs[cnt++] = n / d;
            }
        }
    }

    /* The number of subintervals equals the number of divisors. */
    seq_t k = (seq_t)cnt;

    /* Sort the divisor list so that differences are computed in order. */
    qsort(divs, cnt, sizeof(uint64_t), cmp_uint64);

    /* Compute the sum of squares of the lengths of the subintervals. */
    seq_t prev = (seq_t)0;
    seq_t sumSq = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t d = (seq_t)divs[i];
        seq_t len = d - prev;
        sumSq += len * len;
        prev = d;
    }

    /* Compute floor of the sample variance. */
    seq_t nSeq   = (seq_t)n;
    seq_t numer  = k * sumSq - nSeq * nSeq;
    seq_t denom  = k * (k - 1);
    seq_t result = numer / denom;   /* integer division truncates downward */

    free(divs);
    return result;
}
