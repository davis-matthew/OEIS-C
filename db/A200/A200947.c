/*
 * A200947  Sequence A007924 expressed in decimal.
 * https://oeis.org/A200947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal(A007924(n)): greedy representation of n in place
 *            values 1, 2, 3, 5, 7, 11, ... (1 and the primes), read as a
 *            binary number; digit pi(p) is set for each prime p subtracted,
 *            digit 0 for a remainder 1. Primes come from a sieve. a(0) = 0
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 130, u64 n <= 310,
 *            u128 n <= 718, big any n
 * Validated: standard, pass, 66 terms through n = 9667 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    uint64_t N = (uint64_t)n;
    bool *comp = (bool *)calloc(N + 1, sizeof(bool));
    uint64_t *pic = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    uint64_t i, j, cnt = 0;
    comp[0] = true;
    if (N >= 1) comp[1] = true;
    for (i = 2; i * i <= N; i++)
        if (!comp[i])
            for (j = i * i; j <= N; j += i) comp[j] = true;
    for (i = 0; i <= N; i++) {
        if (!comp[i]) cnt++;
        pic[i] = cnt;
    }
    unsigned char *dig = (unsigned char *)calloc(cnt + 1, 1);
    uint64_t cur = N, top = 0;
    bool first = true;
    while (cur >= 2) {
        uint64_t p = cur;
        while (comp[p]) p--;
        uint64_t pos = pic[p];
        dig[pos] = 1;
        if (first) { top = pos; first = false; }
        cur -= p;
    }
    if (cur == 1) dig[0] = 1;
    seq_t result = 0;
    uint64_t k = top + 1;
    while (k > 0) {
        k--;
        result = result * 2 + (seq_t)dig[k];
    }
    free(comp);
    free(pic);
    free(dig);
    return result;
}
