/*
 * A200537  G.f. satisfies: A(x) = exp( Sum_{n>=1} [Sum_{k=0..2*n}
 *          A200536(n,k)^2 * x^k] / A(x)^n * x^n/n ), where A200536(n,k) is
 *          the coefficient of x^k in (1+3*x+2*x^2)^n.
 * https://oeis.org/A200537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Ncoeff[n] + 4*a(n-2) – 4*a(n-4) Initial values: a(0)=1,
 *            a(1)=1, a(2)=9, a(3)=13, a(4)=40 (where Ncoeff[0..8] =
 *            {1,1,5,9,8,24,20,16,16})
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static const unsigned short Ncoeff[9] = {1, 1, 5, 9, 8, 24, 20, 16, 16};

seq_t a(idx_t n)
{
    /* Allocate temporary array for dynamic programming */
    size_t sz = (size_t)n + 1;
    seq_t  *seq = mem_new(seq_t, sz);
    if (!seq) return (seq_t)0;        /* allocation failure -> zero */

    /* Base cases */
    if (sz > 0) seq[0] = (seq_t)1;
    if (sz > 1) seq[1] = (seq_t)1;
    if (sz > 2) seq[2] = (seq_t)9;
    if (sz > 3) seq[3] = (seq_t)13;
    if (sz > 4) seq[4] = (seq_t)40;

    /* Recurrence for n >= 5 */
    for (size_t i = 5; i < sz; ++i)
    {
        seq_t term = (seq_t)(i < 9 ? Ncoeff[i] : 0);
        term += mul_small(seq[i-2], 4);
        term -= mul_small(seq[i-4], 4);
        seq[i] = term;
    }

    seq_t result = seq[n];
    mem_free(seq);
    return result;
}
