/*
 * A066255  a(n) = A066251(A066251(n)).
 * https://oeis.org/A066255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A066251(A066251(n)), where A066251(n) = (if n is odd then
 *            prime((n+1)/2) else composite(n/2)) - 1; computed with a growing
 *            sieve
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A066255_pr = NULL;
static uint64_t *A066255_co = NULL;
static uint64_t A066255_np = 0, A066255_nc = 0, A066255_lim = 0;

static void A066255_build(uint64_t lim)
{
    char *s = (char *)malloc(lim + 1);
    uint64_t i, j;
    free(A066255_pr);
    free(A066255_co);
    A066255_pr = (uint64_t *)malloc((lim + 1) * sizeof(uint64_t));
    A066255_co = (uint64_t *)malloc((lim + 1) * sizeof(uint64_t));
    for (i = 0; i <= lim; i++) s[i] = 1;
    for (i = 2; i * i <= lim; i++)
        if (s[i])
            for (j = i * i; j <= lim; j += i) s[j] = 0;
    A066255_np = 0;
    A066255_nc = 0;
    for (i = 2; i <= lim; i++) {
        if (s[i]) A066255_pr[A066255_np++] = i;
        else A066255_co[A066255_nc++] = i;
    }
    free(s);
    A066255_lim = lim;
}

static uint64_t A066255_f(uint64_t n)
{
    uint64_t k;
    if (A066255_lim == 0) A066255_build(1024);
    if (n & 1) {
        k = (n + 1) / 2;
        while (A066255_np < k) A066255_build(A066255_lim * 2);
        return A066255_pr[k - 1] - 1;
    } else {
        k = n / 2;
        while (A066255_nc < k) A066255_build(A066255_lim * 2);
        return A066255_co[k - 1] - 1;
    }
}

seq_t a(idx_t n)
{
    uint64_t m = A066255_f((uint64_t)n);
    return (seq_t)A066255_f(m);
}
