/*
 * A083059  a(n) is the number of natural numbers k such that A078496(k)=n.
 * https://oeis.org/A083059
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{k>3 : A078496(k)=n}; A078496(k) is the smallest prime
 *            p>k with 2k-p prime. Computed by a sieve and a direct search
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 102 terms through n = 9717 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;
    size_t N = (size_t)n;
    char *comp = (char *)calloc(N + 2, 1);
    if (!comp)
        return 0;
    comp[0] = 1;
    comp[1] = 1;
    for (size_t i = 2; i * i <= N; i++)
        if (!comp[i])
            for (size_t j = i * i; j <= N; j += i)
                comp[j] = 1;
    if (comp[N]) {
        free(comp);
        return 0;
    }
    size_t count = 0;
    for (size_t k = N / 2 + 1; k < N; k++) {
        if (k < 4)
            continue;
        size_t q = 2 * k - N;
        if (comp[q])
            continue;
        bool smaller = false;
        for (size_t p = k + 1; p < N; p++) {
            if (!comp[p] && !comp[2 * k - p]) {
                smaller = true;
                break;
            }
        }
        if (!smaller)
            count++;
    }
    free(comp);
    return (seq_t)count;
}
