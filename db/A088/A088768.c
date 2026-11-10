/*
 * A088768  a(n) = (A087682(n)-1)/2.
 * https://oeis.org/A088768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A087682(n)-1)/2, where A087682 lists m with m-8 and m+8
 *            both prime; computed by a doubling sieve search
 * Range:     u8 n <= 24, u16 n <= 1525, u32 n <= 5300, u64 n <= 5300,
 *            u128 n <= 5300, big any n
 * Validated: standard, pass, 71 terms through n = 5178 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A088768_terms = NULL;
static size_t A088768_count = 0;
static uint64_t A088768_limit = 0;

static void A088768_build(uint64_t limit)
{
    uint64_t top = limit + 8;
    unsigned char *comp = (unsigned char *)calloc((size_t)top + 1, 1);
    uint64_t i, j;
    size_t cnt = 0;
    comp[0] = 1;
    if (top >= 1) comp[1] = 1;
    for (i = 2; i * i <= top; i++)
        if (!comp[i])
            for (j = i * i; j <= top; j += i) comp[j] = 1;
    free(A088768_terms);
    A088768_terms = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)limit + 1));
    for (i = 10; i <= limit; i++)
        if (!comp[i - 8] && !comp[i + 8])
            A088768_terms[cnt++] = i;
    A088768_count = cnt;
    A088768_limit = limit;
    free(comp);
}

seq_t a(idx_t n)
{
    if (A088768_limit == 0) A088768_build(4096);
    while (A088768_count < (size_t)n)
        A088768_build(A088768_limit * 2);
    return (seq_t)((A088768_terms[n - 1] - 1) / 2);
}
