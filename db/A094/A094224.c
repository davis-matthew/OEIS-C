/*
 * A094224  Number of digits in the least n-parasitic number.
 * https://oeis.org/A094224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = min_{k=1..9} ord_10((10*n-1)/gcd(k,10*n-1))
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   gcd, multiplicative_order_of_10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where ord_10(m) is the multiplicative order of 10 modulo m. */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/multiplicative_order_of_10.h"

seq_t a(idx_t n)
{
    /* Compute M = 10*n - 1.  The intermediate cast ensures the
       multiplication is performed in seq_t to avoid overflow. */
    seq_t M = ((seq_t)10) * n - (seq_t)1;

    seq_t best = (seq_t)0;  /* zero means uninitialized */
    for (idx_t k = (idx_t)1; k <= (idx_t)9; ++k) {
        seq_t g = gcd((seq_t)k, M);
        seq_t m = M / g;
        seq_t ord = multiplicative_order_of_10(m);
        if (best == (seq_t)0 || ord < best)
            best = ord;
    }
    return best;
}
