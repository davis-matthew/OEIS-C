/*
 * A143112  A051731 * A032742 = sum of largest proper divisors of the divisors
 *          of n.
 * https://oeis.org/A143112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} lpd(d), where lpd(d) = (d==1 ? 1 : d / spf(d))
 * Range:     u8 n <= 179, u16 n <= 9614, u32 n <= 9614, u64 n <= 9614,
 *            u128 n <= 9614, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* lpd(d) is the largest proper divisor of d. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

static idx_t *spf_cache = NULL;
static idx_t spf_cache_size = 0;

static void ensure_spf(idx_t limit)
{
    if (spf_cache && spf_cache_size > limit)
        return;

    if (spf_cache) {
        free(spf_cache);
    }

    build_spf(limit, &spf_cache);
    /* The SPF array returned is of size limit+1.  Ensure the first two entries. */
    spf_cache[0] = 0;
    if (limit >= 1)
        spf_cache[1] = 1;

    spf_cache_size = limit;   /* last valid index */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* offset starts at 1 */

    ensure_spf(n);

    /* factor n into primes and exponents, using the SPF array */
    idx_t pf[20], pf_exp[20];
    size_t pf_cnt = 0;
    idx_t temp = n;
    while (temp > 1) {
        idx_t p = spf_cache[temp];
        if (p == 0)          /* safety: treat as prime if SPF entry absent */
            p = temp;
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        pf[pf_cnt] = p;
        pf_exp[pf_cnt] = e;
        ++pf_cnt;
    }

    /* generate all divisors of n */
    idx_t divs[512];           /* 512 > maximum divisor count for n <= 10^4 */
    size_t div_cnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < pf_cnt; ++i) {
        idx_t p = pf[i];
        idx_t e = pf_exp[i];
        size_t old_cnt = div_cnt;
        idx_t pow = 1;
        for (idx_t k = 1; k <= e; ++k) {
            pow *= p;
            for (size_t j = 0; j < old_cnt; ++j) {
                divs[div_cnt++] = divs[j] * pow;
            }
        }
    }

    /* sum largest proper divisors of all divisors of n */
    seq_t sum = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        idx_t d = divs[i];
        seq_t val;
        if (d == 1) {
            val = 1;
        } else {
            val = (seq_t)d / (seq_t)spf_cache[d];
        }
        sum += val;
    }
    return sum;
}
