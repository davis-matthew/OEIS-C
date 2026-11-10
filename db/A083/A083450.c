/*
 * A083450  Positions of records in A084840.
 * https://oeis.org/A083450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = position of n-th record (strict new maximum) in A084840,
 *            where A084840(r) = number of abundant numbers in row r of the
 *            triangle (rows hold (r-1)r/2+1 .. r(r+1)/2); computed by a
 *            divisor-sum sieve
 * Range:     u8 n <= 43, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool find_record(unsigned long long n, unsigned long long *out)
{
    unsigned long long M = 256;
    for (;;) {
        size_t T = (size_t)(M * (M + 1) / 2);
        uint32_t *sg = (uint32_t *)malloc((T + 1) * sizeof(uint32_t));
        if (!sg) exit(1);
        for (size_t i = 0; i <= T; ++i) sg[i] = 0;
        for (size_t i = 1; i <= T; ++i)
            for (size_t j = i; j <= T; j += i)
                sg[j] += (uint32_t)i;
        long long best = -1;
        unsigned long long cnt = 0;
        for (unsigned long long r = 1; r <= M; ++r) {
            size_t lo = (size_t)((r - 1) * r / 2 + 1);
            size_t hi = (size_t)(r * (r + 1) / 2);
            long long c = 0;
            for (size_t k = lo; k <= hi; ++k)
                if (sg[k] > 2 * (uint64_t)k) ++c;
            if (c > best) {
                best = c;
                if (++cnt == n) {
                    free(sg);
                    *out = r;
                    return true;
                }
            }
        }
        free(sg);
        M *= 2;
    }
}

seq_t a(idx_t n)
{
    unsigned long long r = 0;
    find_record((unsigned long long)n, &r);
    return (seq_t)r;
}
