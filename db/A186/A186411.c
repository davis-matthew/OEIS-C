/*
 * A186411  First differences of A186410.
 * https://oeis.org/A186411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A186411
 *
 *  a(n) = 0,                                    n = 0
 *  a(n) = 1,                                    n = 1
 *  a(n) = 1 + 4 * 5^(A000120(n-1) - 1),          n ≥ 2
 *
 *  Here A000120(k) is the binary weight (number of 1‑bits) of k.
 *  The implementation uses only integer arithmetic and works
 *  for any seq_t/idx_t configuration supported by the build.
 */

#include <stdint.h>

static inline int popcount_u64(idx_t x)
{
    int c = 0;
    while (x)
    {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Count 1-bits in n-1 */
    int pc = popcount_u64(n - 1);

    /* Exponent for 5; pc ≥ 1 for all n ≥ 2 */
    idx_t exp = (idx_t)(pc - 1);

    /* Compute 5^exp in seq_t */
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        pow *= (seq_t)5;

    /* a(n) = 1 + 4 * 5^exp */
    seq_t result = (seq_t)1 + (seq_t)4 * pow;
    return result;
}
