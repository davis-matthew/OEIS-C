/*
 * A206802  a(n) = (1/2)*A185382(n).
 * https://oeis.org/A206802
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*Sum_{j=1..n-1} (P(n)-P(j)), P(j) = j-th odd prime
 *            (A065091)
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 48 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *a206802_pr = NULL;
static size_t a206802_cnt = 0;

static void a206802_ensure(size_t need)
{
    if (a206802_pr && a206802_cnt >= need) return;
    size_t limit = 64;
    for (;;) {
        char *s = (char *)calloc(limit + 1, 1);
        size_t c = 0;
        size_t i, j;
        for (i = 2; i <= limit; i++) {
            if (!s[i]) {
                c++;
                for (j = i * i; j <= limit; j += i) s[j] = 1;
            }
        }
        if (c >= need) {
            uint32_t *p = (uint32_t *)malloc(c * sizeof(uint32_t));
            size_t k = 0;
            for (i = 2; i <= limit; i++) if (!s[i]) p[k++] = (uint32_t)i;
            free(s);
            if (a206802_pr) free(a206802_pr);
            a206802_pr = p;
            a206802_cnt = c;
            return;
        }
        free(s);
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    a206802_ensure((size_t)n + 1);
    uint32_t pn = a206802_pr[n];
    seq_t sum = 0;
    for (size_t j = 1; j < (size_t)n; j++)
        sum += (seq_t)((pn - a206802_pr[j]) / 2);
    return sum;
}
