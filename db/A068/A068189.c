/*
 * A068189  Smallest positive number whose product of digits equals n, or
 *          a(n)=0 if no such number exists, i.e. when n has a prime divisor
 *          greater than 7.
 * https://oeis.org/A068189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   smallest positive integer whose product of digits equals n, or 0
 *            if no such number exists (i.e., when n has a prime factor > 7)
 * Range:     u8 n <= 59, u16 n <= 6249, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)            /* product of digits of 1 is 1 */
        return (seq_t)1;

    unsigned long long temp = (unsigned long long)n;
    unsigned long long e2 = 0, e3 = 0, e5 = 0, e7 = 0;

    /* Factor out 2,3,5,7 only */
    while ((temp & 1ULL) == 0) { e2++; temp >>= 1; }
    while (temp % 3ULL == 0)   { e3++; temp /= 3ULL; }
    while (temp % 5ULL == 0)   { e5++; temp /= 5ULL; }
    while (temp % 7ULL == 0)   { e7++; temp /= 7ULL; }

    /* if any other prime remains, no such number exists */
    if (temp != 1ULL)
        return (seq_t)0;

    /* build a digit multiset */
    int digits[128];
    size_t dcnt = 0;

    /* use 9 = 3^2 */
    unsigned long long cnt9 = e3 / 2;
    for (unsigned long long i = 0; i < cnt9; ++i) digits[dcnt++] = 9;
    e3 -= 2 * cnt9;

    /* use 8 = 2^3 */
    unsigned long long cnt8 = e2 / 3;
    for (unsigned long long i = 0; i < cnt8; ++i) digits[dcnt++] = 8;
    e2 -= 3 * cnt8;

    /* use 6 = 2*3 */
    while (e2 > 0 && e3 > 0) { digits[dcnt++] = 6; --e2; --e3; }

    /* use 7 = 7 */
    while (e7 > 0) { digits[dcnt++] = 7; --e7; }

    /* use 5 = 5 */
    while (e5 > 0) { digits[dcnt++] = 5; --e5; }

    /* use 4 = 2^2 */
    while (e2 >= 2) { digits[dcnt++] = 4; e2 -= 2; }

    /* remaining 3's */
    while (e3 > 0) { digits[dcnt++] = 3; --e3; }

    /* remaining 2's */
    while (e2 > 0) { digits[dcnt++] = 2; --e2; }

    /* sort digits increasingly for minimal numeric value */
    for (size_t i = 1; i < dcnt; ++i) {
        int key = digits[i];
        size_t j = i;
        while (j > 0 && digits[j-1] > key) {
            digits[j] = digits[j-1];
            --j;
        }
        digits[j] = key;
    }

    /* construct the number from the sorted digits */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < dcnt; ++i) {
        result = result * (seq_t)10 + (seq_t)digits[i];
    }

    return result;
}
