/*
 * A167294  Totally multiplicative sequence with a(p) = 2*(p-2) for prime p.
 * https://oeis.org/A167294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(1)=1; a(2k)=0; a(p^e) = (2*(p-2))^e for odd prime p;
 *            multiplicativity
 * Range:     u8 n <= 118, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_spf, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_spf.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 1) return (seq_t)1;
    if ((n & 1U) == 0) return (seq_t)0;   /* even numbers are zero */

    /* smallest‑prime‑factor table, grown on demand */
    static idx_t *spf   = NULL;
    static idx_t spf_lim = 0;

    if (n + 1 > spf_lim) {
        /* fre[e old table if it exists */
        if (spf) free(spf);
        /* build a new table up to n inclusive */
        build_spf(n + 1, &spf);
        spf_lim = n + 1;
    }

    seq_t result = (seq_t)1;
    idx_t x = n;

    while (x > 1) {
        idx_t p = spf[x];
        idx_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        /* p==2 cannot happen because n is odd, but guard anyway */
        if (p == 2) return (seq_t)0;

        seq_t base = (seq_t)(2 * (((seq_t)p) - (seq_t)2));
        seq_t pow  = seq_pow(base, e);
        result = seq_mul(result, pow);
    }

    return result;
}
