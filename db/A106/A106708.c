/*
 * A106708  a(n) is the concatenation of its nontrivial divisors.
 * https://oeis.org/A106708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the concatenation of the non‑trivial divisors of n
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 119,
 *            u128 n <= 419, big any n
 * Helpers:   pow10_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10_local.h"
static idx_t num_digits(idx_t x)
{
    idx_t d = 0;
    do {
        d++;
        x /= 10;
    } while (x);
    return d;
}

seq_t a(idx_t n)
{
    /* For n < 4 there are no non‑trivial divisors. */
    if (n <= 3) return 0;

    /* Static arrays are large enough for all n up to many millions. */
    idx_t small[128];
    idx_t large[128];
    int scnt = 0, lcnt = 0;

    /* Find divisors in pairs i and n/i. */
    for (idx_t i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            small[scnt++] = i;
            idx_t j = n / i;
            if (j != i)          /* avoid duplicate for perfect square */
                large[lcnt++] = j;
        }
    }

    /* Combine the two lists into a single increasing list. */
    idx_t divs[256];
    int pos = 0;
    for (int k = 0; k < scnt; ++k)   /* small divisors are already sorted */
        divs[pos++] = small[k];
    for (int k = lcnt - 1; k >= 0; --k) /* reverse large to get ascending order */
        divs[pos++] = large[k];

    /* Concatenate the divisors as decimal digits. */
    seq_t res = 0;
    for (int k = 0; k < pos; ++k) {
        idx_t d = divs[k];
        idx_t dlen = num_digits(d);
        seq_t mult = pow10_local(dlen);
        res = res * mult + (seq_t)d;
    }

    return res;
}
