/*
 * A097636  Odd composites of the form A024675(k) or 1+A024675(k), any k.
 * https://oeis.org/A097636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   sieve; take v = (prime(k+1)+prime(k+2))/2, add 1 if v is even,
 *            and keep v when it is composite (the odd composites among
 *            5,7,9,13,15,19,...)
 * Range:     u8 n <= 36, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *A097636_terms = NULL;
static size_t A097636_count = 0;

static void A097636_build(size_t need)
{
    uint64_t limit = 256;
    for (;;) {
        unsigned char *comp = (unsigned char *)calloc((size_t)limit + 1, 1);
        uint64_t i, j;
        size_t cap = (size_t)limit / 2 + 4, cnt = 0;
        uint64_t *terms;
        uint64_t prev = 0;
        if (!comp) return;
        comp[0] = comp[1] = 1;
        for (i = 2; i * i <= limit; i++)
            if (!comp[i])
                for (j = i * i; j <= limit; j += i) comp[j] = 1;
        terms = (uint64_t *)malloc(cap * sizeof(uint64_t));
        if (!terms) { free(comp); return; }
        for (i = 3; i <= limit; i += 2) {
            if (comp[i]) continue;
            if (prev) {
                uint64_t v = (prev + i) / 2;
                if (!(v & 1)) v++;
                if (comp[v] && cnt < cap) terms[cnt++] = v;
            }
            prev = i;
        }
        free(comp);
        if (cnt >= need) {
            free(A097636_terms);
            A097636_terms = terms;
            A097636_count = cnt;
            return;
        }
        free(terms);
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if ((size_t)n > A097636_count)
        A097636_build((size_t)n);
    return (seq_t)A097636_terms[(size_t)n - 1];
}
