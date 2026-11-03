/*
 * A038398  Concatenate first n cubes in reverse order.
 * https://oeis.org/A038398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenate_{k=1..n} (k^3) in reverse order
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is formed by writing each k³ after the previous one, starting
   with k = n and ending with k = 1.  For example:
   a(3) = 3³‖2³‖1³ = 27‖8‖1 = 2781.  */

/* Required utilities */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1; n = 0 is not used, return 0 defensively */
    if (n < 1)
        return 0;

    seq_t result = 0;
    for (idx_t k = n; k > 0; --k)
    {
        seq_t kk    = (seq_t)k;
        seq_t cube  = kk * kk * kk;                       /* k³ */
        int    ndig = num_digits(cube);                    /* decimal digits of k³ */
        seq_t mult  = pow10((idx_t)ndig);                 /* 10^ndig */
        result = result * mult + cube;                    /* append cube */
    }
    return result;
}
