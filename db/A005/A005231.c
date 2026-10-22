/*
 * A005231  Odd abundant numbers (odd numbers m whose sum of divisors exceeds
 *          2m).
 * https://oeis.org/A005231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve sigma(m) over odd m up to a bound and collect the odd m
 *            with sigma(m) > 2m (search)
 * Range:     u8 none, u16 n <= 143, u32 n <= 9754, u64 n <= 9754,
 *            u128 n <= 9754, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static uint64_t *terms = NULL;
static idx_t terms_len = 0;
static uint64_t sieved_limit = 0;

static void sieve_up_to(uint64_t limit) {
    uint64_t half = limit / 2 + 1;
    uint64_t *sig = (uint64_t *)calloc((size_t)half, sizeof(uint64_t));
    uint64_t d, m, i;
    idx_t cnt = 0;
    if (sig == NULL) return;
    for (d = 1; d <= limit; d += 2)
        for (m = d; m <= limit; m += 2 * d)
            sig[m >> 1] += d;
    for (i = 0; i < half; i++) {
        m = 2 * i + 1;
        if (m <= limit && sig[i] > 2 * m) cnt++;
    }
    free(terms);
    terms = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    terms_len = 0;
    for (i = 0; i < half; i++) {
        m = 2 * i + 1;
        if (m <= limit && sig[i] > 2 * m) terms[terms_len++] = m;
    }
    free(sig);
    sieved_limit = limit;
}

seq_t a(idx_t n) {
    while (terms_len < n) {
        uint64_t want = (uint64_t)n * 600 + 2000;
        uint64_t lim = sieved_limit * 2;
        if (lim < want) lim = want;
        sieve_up_to(lim);
    }
    return (seq_t)terms[n - 1];
}
