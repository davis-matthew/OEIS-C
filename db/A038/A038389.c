/*
 * A038389  Let f(n) be the smallest number such that the arithmetic mean (A)
 *          and geometric mean (G) of n and f(n) are both integers; sequence
 *          gives A values.
 * https://oeis.org/A038389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + f(n)) / 2, where f(n) is the smallest positive
 *            integer such that n + f(n) is even and n * f(n) is a perfect
 *            square. f(n) = s if n is odd or s is even, otherwise f(n) = 4 *
 *            s, where s is the product of the distinct primes that divide n
 *            to an odd power (the squarefree part of n)
 * Range:     u8 n <= 234, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the squarefree part of n (product of primes with odd exponents). */
static seq_t
squarefree_part(idx_t n)
{
    idx_t  temp = n;
    seq_t  res  = 1;

    /* factor 2 */
    int cnt = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        cnt++;
    }
    if (cnt & 1) {
        res *= (seq_t)2ULL;
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        cnt = 0;
        while (temp % p == 0) {
            temp /= p;
            cnt++;
        }
        if (cnt & 1) {
            res *= (seq_t)p;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        res *= (seq_t)temp;
    }

    return res;
}

seq_t
a(idx_t n)
{
    if (n == 0) {
        return 0;                 /* undefined for n = 0, but return 0 */
    }

    seq_t f1 = squarefree_part(n);
    seq_t f;

    if (((n & 1ULL) == 0) && ((f1 & 1ULL) == 1)) {
        f = f1 * (seq_t)4ULL;     /* n even, f1 odd → need even f */
    } else {
        f = f1;
    }

    seq_t sum = ((seq_t)n) + f;
    return sum / (seq_t)2ULL;
}
