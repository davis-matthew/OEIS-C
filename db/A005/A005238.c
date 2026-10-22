/*
 * A005238  Numbers k such that k, k+1 and k+2 have the same number of
 *          divisors.
 * https://oeis.org/A005238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   sieve of tau(k) (number of divisors); a(n) is the n-th k with
 *            tau(k) = tau(k+1) = tau(k+2)
 * Range:     u8 n <= 10, u16 n <= 944, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *terms = NULL;
static idx_t nterms = 0, tcap = 0;
static uint64_t lim = 0; /* all k <= lim have been checked */

static void push_term(uint64_t k) {
    if (nterms == tcap) {
        idx_t nc = tcap ? tcap * 2 : 1024;
        terms = (uint64_t *)realloc(terms, (size_t)nc * sizeof(uint64_t));
        tcap = nc;
    }
    terms[nterms++] = k;
}

static void extend(uint64_t newlim) {
    uint64_t M = newlim + 2, d, m, k;
    uint16_t *tau = (uint16_t *)calloc((size_t)(M + 1), sizeof(uint16_t));
    for (d = 1; d <= M; d++)
        for (m = d; m <= M; m += d)
            tau[m]++;
    for (k = lim + 1; k <= newlim; k++)
        if (tau[k] == tau[k + 1] && tau[k + 1] == tau[k + 2])
            push_term(k);
    free(tau);
    lim = newlim;
}

seq_t a(idx_t n) {
    while (nterms < n)
        extend(lim ? lim * 2 : 65536);
    return (seq_t)terms[n - 1];
}
