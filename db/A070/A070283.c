/*
 * A070283  a(1) = 1; a(2) = 2; a(n) = [product of digits of a(n-1)]^[product
 *          of digits of a(n-2)] for n > 2.
 * https://oeis.org/A070283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1; a(2)=2; a(n)=digit_prod(a(n-1))^digit_prod(a(n-2)) for
 *            n>2
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   digit_prod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_prod.h"   /* provides static seq_t digit_prod(seq_t) */

/* Fast exponentiation for seq_t; exp is non‑negative and small enough to fit in idx_t. */
static seq_t pow_seq_local(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1u)
            result = result * base;
        base = base * base;
        exp >>= 1u;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 2)
        return (n == 1) ? (seq_t)1 : (seq_t)2;

    /* For n >= 9 the sequence is indeterminate and defined as 0 in the OEIS data. */
    if (n >= 9)
        return (seq_t)0;

    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)2;   /* a(2) */
    seq_t curr = a2;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t base   = digit_prod(a2);               /* product of digits of a(n-1) */
        idx_t  exp   = (idx_t)digit_prod(a1);        /* product of digits of a(n-2) */
        curr = pow_seq_local(base, exp);

        a1 = a2;
        a2 = curr;
    }
    return curr;
}
