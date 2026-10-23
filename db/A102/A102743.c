/*
 * A102743  Expansion of e.g.f. LambertW(-x)/(x*(x-1)).
 * https://oeis.org/A102743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*a(n-1) + (n+1)^(n-1), a(0) = 1;
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* helper: binary exponentiation on seq_t */
/* assumes seq_t supports the usual integer operators (+, -, *, /, %, <<, >>) */
static seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute a(1) .. a(n), using recurrence:
       a(i) = i*a(i-1) + (i+1)^(i-1) */
    seq_t prev = (seq_t)1;           /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t pow_val = seq_pow((seq_t)i + 1, i - 1);  /* (i+1)^(i-1) */
        prev = (seq_t)i * prev + pow_val;
    }
    return prev;
}
