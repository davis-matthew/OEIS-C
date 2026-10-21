/*
 * A056143  Concatenate ... floor[n/100], floor[n/10], n.
 * https://oeis.org/A056143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat( floor(n/100), floor(n/10), n )
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digits_of_seq_t, pow10_of_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digits_of_seq_t.h"
#include "../utils/pow10_of_int.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;           /* cast index to seq_t */
    seq_t comp[3];

    /* Components to concatenate */
    comp[0] = x / 100;          /* floor(n/100) */
    comp[1] = x / 10;           /* floor(n/10) */
    comp[2] = x;                /* n itself */

    seq_t result = 0;

    for (int i = 0; i < 3; ++i) {
        seq_t part = comp[i];
        /* Skip leading zero components */
        if (result == 0 && part == 0)
            continue;

        int d = digits_of_seq_t(part);
        result = result * pow10_of_int(d) + part;
    }

    return result;
}
