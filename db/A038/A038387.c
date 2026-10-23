/*
 * A038387  a(n) is the smallest number such that the arithmetic mean (A) and
 *          geometric mean (G) of n and a(n) are both integers.
 * https://oeis.org/A038387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime factors of n: * if p == 2: * a *= 4
 *            when exponent e is even * a *= 2 when exponent e is odd * else
 *            (p > 2): * a *= p when exponent e is odd * a *= 1 when exponent
 *            e is even * This gives the smallest m such that (n + m)/2 and
 *            √(n·m) are integers
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* not used, but safe */
    seq_t res = 1;
    idx_t tmp = n;
    /* handle factor 2 separately */
    idx_t e = 0;
    while ((tmp & 1ULL) == 0) {
        e++;
        tmp >>= 1;   /* divide by 2 */
    }
    if (e > 0) {
        if ((e & 1ULL) == 0)       /* even exponent */
            res *= (seq_t)4;
        else                       /* odd exponent */
            res *= (seq_t)2;
    }
    /* factor odd primes */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        e = 0;
        while (tmp % p == 0) {
            e++;
            tmp /= p;
        }
        if (e & 1ULL)               /* odd exponent */
            res *= (seq_t)p;
    }
    /* remaining prime factor, if any, has exponent 1 */
    if (tmp > 1)
        res *= (seq_t)tmp;
    return res;
}
