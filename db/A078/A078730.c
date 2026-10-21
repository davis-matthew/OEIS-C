/*
 * A078730  Sum of products of two successive divisors of n.
 * https://oeis.org/A078730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{tau(n)-1} d_i*d_{i+1}, where d_1<d_2<...<d_tau
 *            are the divisors of n
 * Range:     u8 n <= 19, u16 n <= 287, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gen_div, insertion_sort
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gen_div.h"
#include "../utils/insertion_sort.h"

#define MAX_DIVISORS 8192

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* Factor n into prime powers */
    idx_t temp = n;
    int exps[64];
    seq_t primes[64];
    int pcnt = 0;

    if ((temp & 1) == 0) {            /* factor 2 */
        int e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        primes[pcnt] = (seq_t)2;
        exps[pcnt] = e;
        ++pcnt;
    }
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt] = (seq_t)p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (temp > 1) {
        primes[pcnt] = (seq_t)temp;
        exps[pcnt] = 1;
        ++pcnt;
    }

    /* Generate all divisors */
    static seq_t divs[MAX_DIVISORS];
    size_t len = 0;
    gen_div(0, (seq_t)1, exps, primes, pcnt, divs, &len);

    /* Sort the divisors */
    insertion_sort(divs, len);

    /* Compute sum of successive products */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i + 1 < len; ++i) {
        sum = sum + divs[i] * divs[i + 1];
    }
    return sum;
}
