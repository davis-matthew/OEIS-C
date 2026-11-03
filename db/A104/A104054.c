/*
 * A104054  Numbers which are the sum of three positive cubes and divisible by
 *          31.
 * https://oeis.org/A104054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer that is divisible by 31 and
 *            representable as a sum of three positive integer cubes
 * Range:     u8 n <= 2, u16 n <= 272, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9801 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* All sums a^3 + b^3 + c^3 (1 <= a <= b <= c) up to a limit L are marked in
   a table, and the multiples of 31 among them, in order, are the terms; L
   doubles until there are n. (The original tested each multiple of 31 with
   a double loop and cube roots in seq_t, which timed out.) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *T = NULL;
    static size_t cnt = 0;
    static uint64_t L = 0;
    if (n == 0) return (seq_t)0;
    while (cnt < (size_t)n) {
        L = L ? 2 * L : 1 << 16;
        unsigned char *mark = (unsigned char *)calloc(L + 1, 1);
        if (!mark) abort();
        for (uint64_t a = 1; 3 * a * a * a <= L; ++a)
            for (uint64_t b = a; a * a * a + 2 * b * b * b <= L; ++b)
                for (uint64_t c = b; a * a * a + b * b * b + c * c * c <= L; ++c)
                    mark[a * a * a + b * b * b + c * c * c] = 1;
        size_t k = 0;
        for (uint64_t x = 31; x <= L; x += 31) k += mark[x];
        mem_free(T);
        T = mem_new(seq_t, k ? k : 1);
        if (!T) abort();
        cnt = 0;
        for (uint64_t x = 31; x <= L; x += 31)
            if (mark[x]) T[cnt++] = (seq_t)x;
        free(mark);
    }
    return T[n - 1];
}
