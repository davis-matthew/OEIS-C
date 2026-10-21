/*
 * A221589  Equals one maps: number of n X 4 binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          king-move neighbors in a random 0..3 n X 4 array.
 * https://oeis.org/A221589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16^n - 2401*2^(2*n-9) for n > 4; a(1)=8, a(2)=180,
 *            a(3)=3760, a(4)=64324
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements the OEIS sequence A221589.  */
#include <stdint.h>

/* fast binary exponentiation for integer types that support
   addition, multiplication, and bitwise shift. */
static inline seq_t seq_pow_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
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
    /* Handle the four small indices explicitly. */
    switch (n)
    {
        case 1:  return (seq_t)8;
        case 2:  return (seq_t)180;
        case 3:  return (seq_t)3760;
        case 4:  return (seq_t)64324;
        default: break;
    }

    /* For n >= 5 use the closed‑form formula. */
    seq_t pow16 = seq_pow_uint((seq_t)16, n);               /* 16^n */
    idx_t exp2  = 2 * n - 9;                               /* exponent for 2^(2*n-9) */
    seq_t pow2  = seq_pow_uint((seq_t)2, exp2);             /* 2^(2*n-9) */
    seq_t sub   = (seq_t)2401 * pow2;                      /* 2401 * 2^(2*n-9) */

    return pow16 - sub;
}
