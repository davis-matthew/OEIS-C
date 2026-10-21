/*
 * A069659  Largest n-digit perfect power.
 * https://oeis.org/A069659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(sqrt(10^n – 1))^2 – largest n‑digit perfect power
 *            (always a square)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 1)           /* offset is 1 – behaviour for n < 1 is irrelevant */
        return (seq_t)0;

    /* compute 10^n as a seq_t value */
    seq_t tenpow = pow10(n);

    /* largest n‑digit number: 10^n – 1 */
    seq_t max_num = tenpow - (seq_t)1;

    /* floor of the square root of that number */
    seq_t root = floor_sqrt(max_num);

    /* return its square, the largest perfect power with n digits */
    return root * root;
}
