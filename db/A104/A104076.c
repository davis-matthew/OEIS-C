/*
 * A104076  If k(m) is the m-th divisor (when the divisors are ordered by
 *          size) of n, then a(n) = gcd(k(1)+k(2), k(2)+k(3), k(3)+k(4), ...,
 *          k(j-1)+k(j)), where j is the number of divisors of n.
 * https://oeis.org/A104076
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = GCD of the sums of consecutive sorted divisors of n
 *            (k(1)+k(2), k(2)+k(3), … , k(j-1)+k(j)), where k(i) are the
 *            divisors of n sorted by size. The sequence starts at a(2)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   cmp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/cmp_seq.h"

/* Euclidean algorithm for the GCD of two seq_t values. */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 2. */
    if (n < (idx_t)2) return (seq_t)0;

    /* An upper bound on the number of divisors for n <= 10000
       is 128; we use 256 to be safe. */
    enum { MAX_DIVISORS = 256 };
    seq_t divs[MAX_DIVISORS];
    idx_t count = 0;

    /* Enumerate divisors up to sqrt(n) and store both d and n/d. */
    for (idx_t d = (idx_t)1; d * d <= n; ++d) {
        if (n % d == (idx_t)0) {
            /* Add the small divisor. */
            if (count < MAX_DIVISORS) {
                divs[count++] = (seq_t)d;
            }
            /* Add the paired large divisor if distinct. */
            idx_t other = n / d;
            if (other != d && count < MAX_DIVISORS) {
                divs[count++] = (seq_t)other;
            }
        }
    }

    /* Sort the collected divisors. */
    if (count > 1) {
        qsort(divs, (size_t)count, sizeof(seq_t), cmp_seq);
    }

    /* Compute the GCD of sums of adjacent divisors. */
    seq_t g = (seq_t)0;
    if (count >= 2) {
        g = divs[0] + divs[1];
        for (idx_t i = 1; i + (idx_t)1 < count; ++i) {
            seq_t s = divs[i] + divs[i + (idx_t)1];
            g = seq_gcd(g, s);
        }
    }

    return g;
}
