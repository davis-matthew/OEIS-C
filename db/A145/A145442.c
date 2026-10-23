/*
 * A145442  Multipliers of SI prefixes, in increasing order.
 * https://oeis.org/A145442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multipliers of SI prefixes: a(n) = 10^e(n), where the exponents e(n) are
   1, 2, 3, 6, 9, 12, 15, ... (i.e. e(1)=1, e(2)=2, e(3)=3, and for n>=4
   e(n)=3*(n-2). */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* SI prefixes exist for exponents 1, 2, 3 and then every third exponent
       thereafter.  Thus for n = 1, 2, 3 we return 10^n; for n >= 4 we return
       10^(3*(n-2)).  This covers all listed terms and any larger n.  */
    if (n <= 3)
        return pow10(n);
    else
        return pow10(3 * (n - 2));
}
