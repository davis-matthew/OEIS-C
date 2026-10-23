/*
 * A034077  Decimal part of n-th root of a(n) starts with digit 0.
 * https://oeis.org/A034077
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 103, u64 n <= 103, u128 n <= 103,
 *            big any n
 * Helpers:   big_pow, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A034077 – Decimal part of n‑th root of a(n) starts with digit 0.
    a(n) = minimal integer m>1 such that the fractional part of m^(1/n)
    is less than 0.1. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/pow10.h"

/* Return a(n) for n>=2.  For n<2 the value is undefined; we return 0. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* undefined for the offset */

    /* For n>=8 the smallest integer is always 2. */
    if (n >= 8)
        return 2;

    /* Otherwise search for the smallest k such that (k^n+1) < (k+0.1)^n. */
    for (idx_t k = 1; ; k++)
    {
        /* compute k^n */
        seq_t k_pow = big_pow((seq_t)k, (seq_t)n);

        /* candidate m = k^n + 1 */
        seq_t m = k_pow + 1;

        /* (k+0.1)^n = (10k+1)^n / 10^n  -->  compare with m */
        seq_t base   = (seq_t)(10 * k + 1);
        seq_t upper  = big_pow(base, (seq_t)n); /* (10k+1)^n   */
        seq_t pow10n = pow10(n);                /* 10^n        */
        /* integer division gives floor((10k+1)^n / 10^n) = floor((k+0.1)^n) */
        seq_t floor_upper = upper / pow10n;

        if (floor_upper >= m)
            return m;            /* first m satisfying the test */
    }
}
