/*
 * A007668  Numbers k such that phi(k) divides sigma(k) and sigma(k)/k >
 *          sigma(m)/m for all m < k.
 * https://oeis.org/A007668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Sieve sigma(k) and phi(k), keep the numbers k where
 *            sigma(k)/k sets a new record over all m < k (superabundant
 *            numbers, A004394), and of those keep the ones where phi(k)
 *            divides sigma(k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t terms[64];
static idx_t nterms = 0;
static uint64_t searched = 0;

static void search_to(uint64_t L) {
    uint64_t *sig = (uint64_t *)calloc(L + 1, sizeof(uint64_t));
    uint64_t *ph = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
    uint64_t i, m, bs = 0, bk = 1;
    if (!sig || !ph) { free(sig); free(ph); return; }
    for (i = 1; i <= L; i++) {
        for (m = i; m <= L; m += i) sig[m] += i;
        ph[i] = i;
    }
    for (i = 2; i <= L; i++)
        if (ph[i] == i)
            for (m = i; m <= L; m += i) ph[m] -= ph[m] / i;
    nterms = 0;
    for (i = 1; i <= L; i++) {
        if (bs == 0 || sig[i] * bk > bs * i) {
            bs = sig[i];
            bk = i;
            if (sig[i] % ph[i] == 0 && nterms < 64) terms[nterms++] = i;
        }
    }
    searched = L;
    free(sig);
    free(ph);
}

seq_t a(idx_t n) {
    uint64_t L = searched ? searched : ((uint64_t)1 << 19);
    while (nterms < n) {
        if (searched >= L) {
            if (L >= ((uint64_t)1 << 23)) return (seq_t)0;
            L *= 2;
        }
        search_to(L);
    }
    return (seq_t)terms[n - 1];
}
