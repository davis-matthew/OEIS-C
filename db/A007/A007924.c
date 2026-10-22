/*
 * A007924  The number n written using the greedy algorithm in the base where
 *          the values of the places are 1 and primes.
 * https://oeis.org/A007924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   greedy representation in place values 1, 2, 3, 5, 7, 11, ... (1
 *            and the primes), written as a decimal number of 0/1 digits:
 *            repeatedly subtract the largest prime p <= remainder, setting
 *            digit pi(p); a remainder of 1 sets digit 0. a(0) = 0. Primes
 *            come from a sieve
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 28, u64 n <= 70, u128 n <= 166,
 *            big any n
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
        result = result * 10 + (seq_t)dig[k];
    }
    free(comp);
    free(pic);
    free(dig);
    return result;
}
