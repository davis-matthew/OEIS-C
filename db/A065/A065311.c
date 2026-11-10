/*
 * A065311  Primes which occur exactly twice in the sequence of a(n) =
 *          prime(n) - prime(n - pi(n)) = A065308(n).
 * https://oeis.org/A065311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(c - pi(c)) where c is the n-th composite with c+1
 *            prime (c = 4, 6, 10, 12, ...); sieve-based search
 * Range:     u8 n <= 19, u16 n <= 947, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 911 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t L = 4096;
    for (;;) {
        unsigned char *comp = (unsigned char *)calloc(L + 2, 1);
        uint32_t *pi = (uint32_t *)malloc((L + 2) * sizeof(uint32_t));
        uint64_t *pr = (uint64_t *)malloc((L + 2) * sizeof(uint64_t));
        uint64_t pc = 0, i, j;
        comp[0] = comp[1] = 1;
        for (i = 2; i * i <= L; i++)
            if (!comp[i])
                for (j = i * i; j <= L; j += i) comp[j] = 1;
        pi[0] = 0;
        for (i = 1; i <= L; i++) {
            if (!comp[i]) pr[pc++] = i;
            pi[i] = (uint32_t)pc;
        }
        uint64_t k = 0;
        seq_t result = 0;
        int found = 0;
        for (i = 4; i < L; i++) {
            if (comp[i] && !comp[i + 1]) {
                k++;
                if (k == (uint64_t)n) {
                    uint64_t m = i - pi[i];
                    if (m <= pc) {
                        result = (seq_t)pr[m - 1];
                        found = 1;
                    }
                    break;
                }
            }
        }
        free(comp);
        free(pi);
        free(pr);
        if (found) return result;
        L *= 2;
    }
}
