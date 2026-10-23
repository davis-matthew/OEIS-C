/*
 * A120841  a(n) = the n-th positive integer which is coprime to the number of
 *          positive divisors of n.
 * https://oeis.org/A120841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer coprime to the number of
 *            positive divisors of n
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   divisor_count_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"  /* if needed, otherwise the function is declared elsewhere */

seq_t a(idx_t n)
{
    /* n is 1‑based; the function is valid for any n ≥ 1 */
    uint64_t tn = (uint64_t)n;          /* convert index to uint64_t for divisor count */
    uint64_t d = divisor_count_u64(tn); /* number of divisors of n */
    seq_t td = (seq_t)d;                /* convert to seq_t for gcd */

    idx_t  count = 0;           /* how many coprime integers found so far */
    seq_t  cand   = 1;          /* current candidate integer */

    while (1) {
        if (gcd(cand, td) == 1) {        /* candidate coprime to d? */
            count++;
            if (count == n) {
                return cand;             /* nth coprime integer found */
            }
        }
        cand++;
    }
}
