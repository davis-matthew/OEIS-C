/*
 * A165860  Totally multiplicative sequence with a(p) = 39.
 * https://oeis.org/A165860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 39^bigomega(n) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 8191,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bigomega.h" /* seq_t bigomega(idx_t n); */
#include "../utils/pow_uint.h" /* seq_t pow_uint(uint64_t base, idx_t exp); */

seq_t a(idx_t n)
{
    /* For safety, if n==0 (outside the official offset) we return 1,
       since 39^0 = 1 and this keeps the function well‑defined. */
    if (n == 0)
        return pow_uint(39, 0);

    seq_t omega = bigomega(n);      /* total number of prime factors (with multiplicity) */
    return pow_uint(39, (idx_t)omega); /* 39^omega(n) */
}
