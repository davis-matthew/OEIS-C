/*
 * A217774  Palindromic deviation of terms of A006960.
 * https://oeis.org/A217774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A064834(A006960(n)), where A006960(0) = 196, A006960(k+1)
 *            = x + reverse_digits(x) with x = A006960(k); computed on a
 *            decimal digit array
 * Range:     u8 n <= 311, u16 n <= 2390, u32 n <= 2390, u64 n <= 2390,
 *            u128 n <= 2390, big any n
 * Validated: standard, pass, 102 terms through n = 2347 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    size_t cap = (size_t)n + 16;
    unsigned char *d = (unsigned char *)malloc(cap);
    unsigned char *t = (unsigned char *)malloc(cap);
    size_t len = 3;
    d[0] = 6; d[1] = 9; d[2] = 1; /* 196, least significant first */
    for (idx_t s = 0; s < n; s++) {
        unsigned carry = 0;
        for (size_t i = 0; i < len; i++) {
            unsigned v = (unsigned)d[i] + (unsigned)d[len - 1 - i] + carry;
            t[i] = (unsigned char)(v % 10);
            carry = v / 10;
        }
        if (carry) t[len++] = (unsigned char)carry;
        unsigned char *sw = d; d = t; t = sw;
    }
    unsigned long long res = 0;
    for (size_t i = 0; i < len / 2; i++) {
        unsigned a1 = d[i], a2 = d[len - 1 - i];
        res += (a1 > a2) ? (a1 - a2) : (a2 - a1);
    }
    free(d);
    free(t);
    return (seq_t)res;
}
