/*
 * A088770  a(n) = (A087683(n)-1)/2.
 * https://oeis.org/A088770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A087683(n)-1)/2, where A087683(n) = n-th m with m-10 and
 *            m+10 both prime (sieve search)
 * Range:     u8 n <= 31, u16 n <= 2043, u32 n <= 4710, u64 n <= 4710,
 *            u128 n <= 4710, big any n
 * Validated: standard, pass, 71 terms through n = 4439 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A088770_terms = NULL;
static size_t A088770_cnt = 0;
static size_t A088770_limit = 0;

static void A088770_build(size_t limit)
{
    unsigned char *s = (unsigned char *)malloc(limit + 21);
    size_t i, j;
    if (!s) return;
    for (i = 0; i <= limit + 20; i++) s[i] = 1;
    s[0] = 0; s[1] = 0;
    for (i = 2; i * i <= limit + 20; i++)
        if (s[i])
            for (j = i * i; j <= limit + 20; j += i) s[j] = 0;
    size_t c = 0;
    for (i = 2; i <= limit; i++)
        if (s[i] && s[i + 20]) c++;
    free(A088770_terms);
    A088770_terms = (uint64_t *)malloc((c + 1) * sizeof(uint64_t));
    A088770_cnt = 0;
    if (A088770_terms) {
        for (i = 2; i <= limit; i++)
            if (s[i] && s[i + 20])
                A088770_terms[A088770_cnt++] = ((uint64_t)i + 10 - 1) / 2;
    }
    A088770_limit = limit;
    free(s);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A088770_limit == 0) A088770_build(1u << 16);
    while ((idx_t)A088770_cnt < n && A088770_terms)
        A088770_build(A088770_limit * 2);
    return (seq_t)A088770_terms[n - 1];
}
