/*
 * A007617  Values not in range of Euler phi function.
 * https://oeis.org/A007617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search -- a(n) = n-th number k >= 3 with no m such that
 *            phi(m) = k (A264739(k) = 0). Since all odd k >= 3 qualify, a(n)
 *            <= 2n+1. phi is sieved up to 7*(2n+1), because m/phi(m) > 7 only
 *            happens for m > 7.4e12
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *nt_list = NULL;   /* cached nontotients in increasing order */
static uint64_t nt_count = 0;
static uint64_t nt_limit = 0;      /* all nontotients <= nt_limit are cached */

static void build(uint64_t L) {
    uint64_t M = 7 * L;
    uint64_t *ph = (uint64_t *)malloc((size_t)(M + 1) * sizeof(uint64_t));
    bool *tot = (bool *)calloc((size_t)(L + 1), sizeof(bool));
    uint64_t i, j, c;
    if (!ph || !tot) { free(ph); free(tot); return; }
    for (i = 0; i <= M; i++) ph[i] = i;
    for (i = 2; i <= M; i++) {
        if (ph[i] == i) {
            for (j = i; j <= M; j += i) ph[j] -= ph[j] / i;
        }
    }
    for (i = 1; i <= M; i++) {
        if (ph[i] <= L) tot[ph[i]] = true;
    }
    free(ph);
    c = 0;
    for (i = 1; i <= L; i++) if (!tot[i]) c++;
    free(nt_list);
    nt_list = (uint64_t *)malloc((size_t)(c + 1) * sizeof(uint64_t));
    nt_count = 0;
    for (i = 1; i <= L; i++) if (!tot[i]) nt_list[nt_count++] = i;
    nt_limit = L;
    free(tot);
}

seq_t a(idx_t n) {
    uint64_t L = 2 * (uint64_t)n + 1;
    if (nt_list == NULL || nt_limit < L || nt_count < n) {
        uint64_t want = L;
        if (want < 2 * nt_limit) want = 2 * nt_limit;
        if (want < 64) want = 64;
        build(want);
    }
    if (nt_list == NULL || nt_count < n) return 0;
    return (seq_t)nt_list[n - 1];
}
