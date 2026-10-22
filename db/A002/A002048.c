/*
 * A002048  Segmented numbers, or prime numbers of measurement.
 * https://oeis.org/A002048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   sieve. Go through the integers in increasing order. Each one not
 *            yet marked becomes the next term. When a term is added, mark
 *            every sum of two or more consecutive terms that ends at it
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *a2048_terms = NULL;
static size_t a2048_cnt = 0;

static void a2048_build(size_t need)
{
    size_t L = need * 8 + 64;
    for (;;) {
        unsigned char *excl = (unsigned char *)calloc(L + 1, 1);
        uint64_t *t = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
        size_t cnt = 0, c;
        if (!excl || !t) { free(excl); free(t); return; }
        for (c = 1; c <= L; c++) {
            if (excl[c]) continue;
            t[cnt++] = (uint64_t)c;
            {
                uint64_t s = (uint64_t)c;
                size_t j = cnt - 1;
                while (j-- > 0) {
                    s += t[j];
                    if (s > (uint64_t)L) break;
                    excl[s] = 1;
                }
            }
        }
        free(excl);
        if (cnt >= need) {
            free(a2048_terms);
            a2048_terms = t;
            a2048_cnt = cnt;
            return;
        }
        free(t);
        L *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if ((size_t)n > a2048_cnt) a2048_build((size_t)n);
    if ((size_t)n > a2048_cnt) return 0;
    return (seq_t)a2048_terms[n - 1];
}
