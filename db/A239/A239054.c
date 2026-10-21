/*
 * A239054  Maximum of the partition function on the set of all partitions of
 *          n.
 * https://oeis.org/A239054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n = 1..7 a(n) = A000041(n) (partition number), for n ≥ 8
 *            a(n) = 5^(n/4) if n ≡ 0 (mod 4) 7·5^((n-5)/4) if n ≡ 1 (mod 4)
 *            11·5^((n-6)/4) if n ≡ 2 (mod 4) 11·7·5^((n-11)/4) if n ≡ 3 (mod
 *            4)
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   p_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required headers for malloc and printf if needed */
#include <stdlib.h>
#include "../utils/p_small.h"

/* compute 5^e for unsigned integer exponent e */
static seq_t pow5(idx_t e)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < e; ++i)
        r *= (seq_t)5;
    return r;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 7)
        return p_small(n);

    idx_t mod = n % 4;
    seq_t val;

    switch (mod)
    {
        case 0:   /* n ≡ 0 (mod 4) */
            val = pow5(n / 4);
            break;
        case 1:   /* n ≡ 1 (mod 4) */
            val = pow5((n - 5) / 4);
            val *= (seq_t)7;
            break;
        case 2:   /* n ≡ 2 (mod 4) */
            val = pow5((n - 6) / 4);
            val *= (seq_t)11;
            break;
        case 3:   /* n ≡ 3 (mod 4) */
            val = pow5((n - 11) / 4);
            val *= (seq_t)7;
            val *= (seq_t)11;
            break;
        default:
            val = (seq_t)0; /* unreachable */
            break;
    }
    return val;
}
