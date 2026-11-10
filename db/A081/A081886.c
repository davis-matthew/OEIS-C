/*
 * A081886  Duplicate of A081530.
 * https://oeis.org/A081886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = lcm(1..n)*(n+1)*(H(n+1)-1) = lcm(1..n) * Sum_{k=1..n}
 *            (n-k+1)/k
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 13, i64 n <= 13, i128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    uint8_t *comp = (uint8_t *)calloc((size_t)(n + 1), 1);
    if (!comp)
        return (seq_t)0;

    seq_t l = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        if (comp[i])
            continue;
        if (i <= n / i) {
            for (idx_t j = i * i; j <= n; j += i)
                comp[j] = 1;
        }
        idx_t pw = i;
        while (1) {
            l *= (seq_t)i;
            if (pw > n / i)
                break;
            pw *= i;
        }
    }
    free(comp);

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t part = l / (seq_t)k;
        part *= (seq_t)(n - k + 1);
        sum += part;
    }
    return sum;
}
