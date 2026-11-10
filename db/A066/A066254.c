/*
 * A066254  a(n) = A066250(A066250(n)).
 * https://oeis.org/A066254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A066250(A066250(n)), where A066250(m) = (m+1 is prime) ?
 *            2*pi(m+1)-1 : 2*((m+1)-pi(m+1)-1)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint8_t  *A066254_flags = NULL;
static uint32_t *A066254_pi    = NULL;
static size_t    A066254_max   = 0;

static void A066254_build(size_t limit)
{
    size_t i, j;
    if (limit < 64) limit = 64;
    free(A066254_flags);
    free(A066254_pi);
    A066254_flags = (uint8_t *)malloc(limit + 1);
    A066254_pi    = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    A066254_max = 0;
    if (!A066254_flags || !A066254_pi)
        return;
    for (i = 0; i <= limit; ++i)
        A066254_flags[i] = 1;
    A066254_flags[0] = 0;
    A066254_flags[1] = 0;
    for (i = 2; i * i <= limit; ++i) {
        if (A066254_flags[i]) {
            for (j = i * i; j <= limit; j += i)
                A066254_flags[j] = 0;
        }
    }
    A066254_pi[0] = 0;
    for (i = 1; i <= limit; ++i)
        A066254_pi[i] = A066254_pi[i - 1] + A066254_flags[i];
    A066254_max = limit;
}

static uint64_t A066250_a(uint64_t n)
{
    size_t k;
    uint32_t p;
    if (n == 0)
        return 0;
    k = (size_t)(n + 1);
    if (k > A066254_max)
        A066254_build(k * 2);
    p = A066254_pi[k];
    if (A066254_flags[k])
        return (uint64_t)p * 2 - 1;
    return (uint64_t)(k - p - 1) * 2;
}

seq_t a(idx_t n)
{
    uint64_t m = A066250_a((uint64_t)n);
    return (seq_t)A066250_a(m);
}
