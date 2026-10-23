/*
 * A186444  The count of numbers <= n for which 3 is an infinitary divisor.
 * https://oeis.org/A186444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/3) - floor(n/9) + floor(n/27) - floor(n/81) + …
 *            (alternating signs)
 * Range:     u8 n <= 1022, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count numbers ≤ n that are multiples of 3 but not multiples of 9.
       This uses the alternating sum of floor(n/3^k). For each k, the term
       contributes + if k is odd and – if k is even. */
    seq_t            result = 0;
    uint64_t         power  = 3;      /* current power of 3 */
    int              sign   = 1;      /* +1 for odd k, –1 for even k */

    while (power <= n)
    {
        seq_t term = (seq_t)(n / power);   /* floor(n/3^k) */
        if (sign > 0)
            result += term;
        else
            result -= term;

        /* Prepare next power of 3, but stop before overflow. */
        if (power > UINT64_MAX / 3)
            break;
        power *= 3;
        sign = -sign;
    }
    return result;
}
