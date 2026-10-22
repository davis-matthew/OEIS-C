/*
 * A003343  Numbers that are the sum of 9 positive 4th powers.
 * https://oeis.org/A003343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve: a(n) is the n-th number m that can be written as a sum of
 *            exactly 9 positive 4th powers; found by a layered reachability
 *            DP up to a growing limit
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *a3343_terms = NULL;
static size_t a3343_nterms = 0;
static uint64_t a3343_limit = 0;

static void a3343_build(uint64_t L)
{
    unsigned char *cur = (unsigned char *)calloc((size_t)(L + 1), 1);
    unsigned char *nxt = (unsigned char *)calloc((size_t)(L + 1), 1);
    uint64_t m, j;
    int k;
    size_t cnt = 0;

    cur[0] = 1;
    for (k = 1; k <= 9; k++) {
        memset(nxt, 0, (size_t)(L + 1));
        for (m = 0; m <= L; m++) {
            if (!cur[m]) continue;
            for (j = 1; ; j++) {
                uint64_t j4 = j * j * j * j;
                if (j4 > L - m) break;
                nxt[m + j4] = 1;
            }
        }
        {
            unsigned char *t = cur;
            cur = nxt;
            nxt = t;
        }
    }

    for (m = 0; m <= L; m++)
        if (cur[m]) cnt++;

    free(a3343_terms);
    a3343_terms = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    a3343_nterms = 0;
    for (m = 0; m <= L; m++)
        if (cur[m]) a3343_terms[a3343_nterms++] = m;
    a3343_limit = L;

    free(cur);
    free(nxt);
}

seq_t a(idx_t n)
{
    while ((idx_t)a3343_nterms < n) {
        uint64_t L;
        if (a3343_limit == 0) {
            L = (uint64_t)n * 32 + 1024;
        } else {
            L = a3343_limit * 2;
        }
        a3343_build(L);
    }
    return (seq_t)a3343_terms[n - 1];
}
