/*
 * A066252  a(n) = A066248(A066248(n)).
 * https://oeis.org/A066252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A066248(A066248(n)), where A066248(n) = 2*pi(n+1) if n+1
 *            is prime, else 2*(n - pi(n+1)) - 1. Computed with a prime sieve
 *            and prefix counts of pi
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static uint8_t *A066252_flag = NULL;
static idx_t *A066252_pi = NULL;
static idx_t A066252_lim = 0;

static void A066252_ensure(idx_t limit)
{
    if (limit <= A066252_lim && A066252_flag)
        return;
    if (limit < 16) limit = 16;
    free(A066252_flag);
    free(A066252_pi);
    A066252_flag = (uint8_t *)malloc((size_t)(limit + 1) * sizeof(uint8_t));
    A066252_pi = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!A066252_flag || !A066252_pi) abort();
    for (idx_t i = 0; i <= limit; ++i) A066252_flag[i] = 1;
    A066252_flag[0] = 0;
    A066252_flag[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (A066252_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                A066252_flag[j] = 0;
        }
    }
    A066252_pi[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        A066252_pi[i] = A066252_pi[i - 1] + (A066252_flag[i] ? 1 : 0);
    A066252_lim = limit;
}

/* A066248 on a table-bounded index */
static idx_t A066252_f(idx_t n)
{
    idx_t m = n + 1;
    idx_t p = A066252_pi[m];
    if (A066252_flag[m])
        return 2 * p;
    return 2 * (m - 1 - p) - 1;
}

seq_t a(idx_t n)
{
    A066252_ensure(2 * n + 4);
    idx_t inner = A066252_f(n);
    return (seq_t)A066252_f(inner);
}
