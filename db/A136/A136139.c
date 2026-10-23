/*
 * A136139  a(n+1)=sopfr(5a(n)+1), with sopfr=A001414. Finishes with a cycle
 *          with length 16.
 * https://oeis.org/A136139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(1) = 1, a(n+1) = sopfr(5*a(n)+1) where sopfr(x) is the sum of
 *            the prime factors of x, counted with multiplicity
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Helper: sum of prime factors (with multiplicity) */
static seq_t sopfr(seq_t n)
{
    seq_t sum = 0;
    if (n <= 1) return sum;

    seq_t rem = n;
    /* trial division using 64‑bit counter so that p ≤ floor_sqrt(rem) fits
       in the counter even for large seq_t values. */
    for (uint64_t p = 2; ; ++p) {
        seq_t pp = (seq_t)p;          /* current divisor candidate */
        seq_t lim = floor_sqrt(rem);  /* sqrt bound for current remainder */
        if (pp > lim) break;          /* no more possible factors */

        while (rem % pp == 0) {
            sum += pp;
            rem /= pp;
        }
        if (rem == 1) break;
    }
    if (rem > 1) sum += rem;           /* last prime factor */
    return sum;
}

/* Public function required by the book */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety: sequence is defined for n≥1 */

    seq_t cur = 1;          /* a(1) */
    if (n == 1) return cur;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = sopfr((seq_t)5 * cur + (seq_t)1);
        cur = next;
    }
    return cur;
}
