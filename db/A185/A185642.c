/*
 * A185642  Nearest integer to average of distinct prime factors of n.
 * https://oeis.org/A185642
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(A008472(n)/A001221(n))
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A008472(n) is the sum of distinct prime factors of n,
   and A001221(n) is the number of distinct prime factors of n. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* for factor_u64 */

/* Computes the nearest integer to the average of the distinct prime
   factors of n.  For prime n this returns n. */
seq_t a(idx_t n)
{
    if (n < 2) /* undefined for n < 2, return 0 */
        return (seq_t)0;

    /* factorize n: obtain distinct primes and their exponents */
    size_t cnt = 0;
    uint64_t pr[64];
    uint64_t ex[64];
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* If n is prime, it will have count==1 and the prime==n */
    if (cnt == 1 && pr[0] == n)
        return (seq_t)n;

    /* Compute sum of distinct primes */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)pr[i];

    /* Round half up: (sum + cnt/2) / cnt */
    seq_t add = (seq_t)cnt / 2;          /* integer division of cnt by 2 */
    seq_t numerator = sum + add;
    seq_t result = numerator / (seq_t)cnt;
    return result;
}
