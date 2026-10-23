/*
 * A169494  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 49 * 48^(n-1) for n >= 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"

/* Helper: binary exponentiation of 48 for exponent exp */
static seq_t pow48(idx_t exp)
{
    seq_t result = seq_one();       /* 1 */
    seq_t base   = (seq_t)48;       /* 48 */
    while (exp > 0)
    {
        if (exp & 1UL)
            result = seq_mul(result, base);
        base = seq_mul(base, base);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();                 /* a(0) = 1 */

    seq_t p = pow48(n - 1);               /* 48^(n-1) */
    return seq_mul((seq_t)49, p);         /* 49 * 48^(n-1) */
}
