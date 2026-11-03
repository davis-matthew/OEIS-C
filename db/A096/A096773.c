/*
 * A096773  a(n) = 4*a(n-2) + 1 with a(1) = 0, a(2) = 3.
 * https://oeis.org/A096773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(n-1)*(3+2*(-1)^n)-1)/3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 3177 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 2^(e) for e < 128 using 128‑bit intermediate,
   otherwise fall back to repeated doubling. */
static seq_t power_of_two(idx_t e)
{
#if defined(__SIZEOF_INT128__)
    if (e < 128)
    {
        unsigned __int128 val = ((unsigned __int128)1) << e;
        /* cast back to seq_t; if seq_t is 128‑bit this is a no‑op,
           otherwise it converts the integer value. */
        return (seq_t)val;
    }
#endif
    seq_t result = 1;
    for (idx_t i = 0; i < e; ++i)
        result <<= 1;          /* repeated doubling */
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 3;

    idx_t e = n - 1;
    seq_t pow2 = power_of_two(e);

    if ((n & 1) == 0) /* n even */
    {
        /* (5*2^(n-1)-1)/3 */
        seq_t tmp = 5 * pow2 - 1;
        return tmp / 3;
    }
    else              /* n odd */
    {
        /* (2^(n-1)-1)/3 */
        seq_t tmp = pow2 - 1;
        return tmp / 3;
    }
}
