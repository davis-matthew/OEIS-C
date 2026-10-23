/*
 * A136165  a(n) = the number of divisors of n that are each not noncoprime to
 *          either the previous or the following divisor of n.
 * https://oeis.org/A136165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of divisors d of n such that gcd(d,prev)=1 and
 *            gcd(d,next)=1 where prev and next are the preceding and
 *            succeeding divisors of n in sorted order. a(1) = 1, a(2) = 2,
 *            a(3) = 2, a(4) = 1, a(5) = 2,
 * Range:     u8 n <= 320, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, cmp_seq_t, divisor_count_u64, factor_u64,
 *            free_seq_array, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/gcd.h"
#include "../utils/cmp_seq_t.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static void gen_divs_rec(size_t idx,
                         uint64_t cur,
                         const uint64_t *primes,
                         const uint64_t *exps,
                         size_t cnt,
                         seq_t *out,
                         size_t *pos)
{
    if (idx == cnt) {
        out[*pos] = (seq_t)cur;
        (*pos)++;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t val = cur;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divs_rec(idx + 1, val, primes, exps, cnt, out, pos);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 */
    uint64_t un = (uint64_t)n;

    /* number of divisors of n */
    size_t dcnt = (size_t)divisor_count_u64(un);

    /* allocate array for the divisors */
    seq_t *divs = alloc_seq_array(dcnt);
    if (!divs) return (seq_t)0;          /* allocation failure */

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(un, primes, exps, &pcnt);

    /* generate all divisors */
    size_t pos = 0;
    gen_divs_rec(0, 1, primes, exps, pcnt, divs, &pos);

    /* sort divisors ascending */
    qsort(divs, dcnt, sizeof(seq_t), cmp_seq_t);

    /* count eligible divisors */
    seq_t count = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        bool ok = true;
        if (i > 0) {
            if (gcd(divs[i], divs[i - 1]) != (seq_t)1)
                ok = false;
        }
        if (i + 1 < dcnt) {
            if (gcd(divs[i], divs[i + 1]) != (seq_t)1)
                ok = false;
        }
        if (ok) ++count;
    }

    free_seq_array(divs);
    return count;
}
