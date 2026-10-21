/*
 * A220985  The left Aurifeuillian factor of 10^(20n+10) + 1.
 * https://oeis.org/A220985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(8n+4) - 10^(7n+4) + 5 * 10^(6n+3) - 2 * 10^(5n+3) + 7
 *            * 10^(4n+2) - 2 * 10^(3n+2) + 5 * 10^(2n+1) - 10^(n+1) + 1
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 1, u128 n <= 4,
 *            big any n
 * Helpers:   mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Compute each term using pow10 and multiply by small constants where needed */
    seq_t result = pow10(8 * n + 4);
    result -= pow10(7 * n + 4);

    result += mul_small(pow10(6 * n + 3), 5);
    result -= mul_small(pow10(5 * n + 3), 2);

    result += mul_small(pow10(4 * n + 2), 7);
    result -= mul_small(pow10(3 * n + 2), 2);

    result += mul_small(pow10(2 * n + 1), 5);
    result -= pow10(n + 1);

    result += 1;

    return result;
}
