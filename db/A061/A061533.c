/*
 * A061533  Limiting sequence of digits in A061584, starting with least
 *          significant digit.
 * https://oeis.org/A061533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n-th digit from the right (least significant first) of
 *            the limiting string of A061584, where the suffix S(1)="6",
 *            S(k+1) = each digit d of S(k) replaced by the decimal digits of
 *            6*d; S(k) is a suffix of S(k+1)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 104 terms through n = 104 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static const char *map[10] = {"0","6","12","18","24","30","36","42","48","54"};
    size_t target = (size_t)n;
    size_t len = 1;
    char *cur = (char *)malloc(2);
    cur[0] = '6';
    cur[1] = '\0';
    while (len < target) {
        size_t newlen = 0;
        for (size_t i = 0; i < len; ++i) {
            int d = cur[i] - '0';
            newlen += (d == 0 || d == 1) ? 1 : 2;
        }
        char *next = (char *)malloc(newlen + 1);
        size_t pos = 0;
        for (size_t i = 0; i < len; ++i) {
            const char *m = map[cur[i] - '0'];
            for (; *m; ++m) next[pos++] = *m;
        }
        next[pos] = '\0';
        free(cur);
        cur = next;
        len = newlen;
    }
    int digit = cur[len - target] - '0';
    free(cur);
    return (seq_t)digit;
}
