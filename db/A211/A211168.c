/*
 * A211168  Exponent of alternating group An.
 * https://oeis.org/A211168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = LCM{1, … , n-1} if n is even LCM{1, … , n-2, n} if n is
 *            odd (n ≥ 3) with a(1)=1, a(2)=1, a(3)=3
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 46, u128 n <= 88,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"   /* seq_t gcd(seq_t, seq_t) */

/* static cache of lcm(1..k) for k = 1 .. max_n */
static seq_t *lcm_tbl = NULL;
static idx_t  max_n   = 0;

/* ensure lcm_tbl is computed up to `up_to` (inclusive)  */
static void
ensure_lcm_tbl(idx_t up_to)
{
    if (max_n >= up_to)
        return;

    size_t new_size = (size_t)(up_to + 1);          /* use 1‑based indexing, ignore index 0 */
    seq_t *tmp = (seq_t *)realloc(lcm_tbl, new_size * sizeof(seq_t));
    lcm_tbl = tmp;

    if (max_n == 0) {                               /* first allocation */
        lcm_tbl[0] = 1;                             /* unused, but keep defined */
        lcm_tbl[1] = (seq_t)1;
        max_n     = 1;
    }

    for (idx_t i = max_n + 1; i <= up_to; ++i) {
        seq_t prev = lcm_tbl[i - 1];
        seq_t g    = gcd(prev, (seq_t)i);
        seq_t div  = prev / g;                     /* division is exact  */
        lcm_tbl[i] = div * (seq_t)i;
    }
    max_n = up_to;
}

seq_t a(idx_t n)
{
    if (n == 1 || n == 2)
        return (seq_t)1;

    if (n == 3)
        return (seq_t)3;

    /* compute lcm(1..k) up to k = n-1 or n-2 (whichever is larger) */
    ensure_lcm_tbl(n - 1);

    if ((n & 1) == 0) {                     /* even n */
        return lcm_tbl[n - 1];
    } else {                                 /* odd n */
        seq_t base = lcm_tbl[n - 2];
        seq_t g    = gcd(base, (seq_t)n);
        seq_t div  = base / g;
        return div * (seq_t)n;
    }
}
