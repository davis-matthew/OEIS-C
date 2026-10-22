/*
 * A004709  Cubefree numbers: numbers that are not divisible by any cube > 1.
 * https://oeis.org/A004709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sieve: a(n) is the n-th number not divisible by any cube k^3 > 1, found by marking multiples of p^3 for primes p. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *cf_list = NULL;
static uint64_t cf_count = 0;

static void cf_build(uint64_t need) {
    uint64_t limit = need + need / 4 + 64;
    for (;;) {
        unsigned char *bad = (unsigned char *)calloc((size_t)(limit + 1), 1);
        uint64_t p, m, cnt = 0;
        for (p = 2; p * p * p <= limit; p++) {
            bool prime = true;
            uint64_t d;
            for (d = 2; d * d <= p; d++) {
                if (p % d == 0) { prime = false; break; }
            }
            if (!prime) continue;
            for (m = p * p * p; m <= limit; m += p * p * p) bad[m] = 1;
        }
        for (m = 1; m <= limit; m++) if (!bad[m]) cnt++;
        if (cnt >= need) {
            uint64_t *lst = (uint64_t *)malloc((size_t)cnt * sizeof(uint64_t));
            uint64_t i = 0;
            for (m = 1; m <= limit; m++) if (!bad[m]) lst[i++] = m;
            free(bad);
            free(cf_list);
            cf_list = lst;
            cf_count = cnt;
            return;
        }
        free(bad);
        limit *= 2;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n > cf_count) cf_build(n);
    return (seq_t)cf_list[n - 1];
}
