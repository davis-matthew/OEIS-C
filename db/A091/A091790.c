/*
 * A091790  Palindromes obtained as the concatenation of three successive
 *          terms of A091789.
 * https://oeis.org/A091790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = concatenation of b(n), b(n+1), b(n+2) in decimal, where b
 *            = A091789: b(1) = 1, b(2) = 2, b(k) = reverse(b(k-1)) ||
 *            reverse(b(k-2))
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 12 terms through n = 12 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    size_t top = (size_t)n + 2;
    char **s = (char **)malloc((top + 1) * sizeof(char *));
    size_t *len = (size_t *)malloc((top + 1) * sizeof(size_t));
    if (!s || !len)
        abort();
    len[1] = 1;
    len[2] = 1;
    s[1] = (char *)malloc(1);
    s[2] = (char *)malloc(1);
    if (!s[1] || !s[2])
        abort();
    s[1][0] = '1';
    s[2][0] = '2';
    for (size_t i = 3; i <= top; ++i) {
        len[i] = len[i - 1] + len[i - 2];
        s[i] = (char *)malloc(len[i]);
        if (!s[i])
            abort();
        size_t p = 0;
        for (size_t j = len[i - 1]; j > 0; --j)
            s[i][p++] = s[i - 1][j - 1];
        for (size_t j = len[i - 2]; j > 0; --j)
            s[i][p++] = s[i - 2][j - 1];
    }
    seq_t v = 0;
    for (size_t k = (size_t)n; k <= (size_t)n + 2; ++k) {
        for (size_t j = 0; j < len[k]; ++j) {
            v = v * 10 + (seq_t)(s[k][j] - '0');
        }
    }
    for (size_t i = 1; i <= top; ++i)
        free(s[i]);
    free(s);
    free(len);
    return v;
}
