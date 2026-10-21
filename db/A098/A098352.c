/*
 * A098352  Multiplication table of the even numbers read by antidiagonals.
 * https://oeis.org/A098352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 4·i·j. Let s = ceil((1 + sqrt(1 + 8n))/2). Then i = n -
 *            (s-2)(s-1)/2 (0‑indexed along the s‑diagonal). Set i = i + 1, j
 *            = s - i, and a(n) = 4·i·j
 * Range:     u8 n <= 7, u16 n <= 1632, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic. */
    seq_t dn = (seq_t)n;

    /* Compute s = ceil((1 + sqrt(1 + 8n))/2). */
    seq_t discrim = 1 + (seq_t)8 * dn;
    seq_t sqrtFloor = floor_sqrt(discrim);
    seq_t s = (1 + sqrtFloor) / 2;          /* floor of the expression */
    while (((s - 1) * s) / 2 < dn)          /* adjust to get ceil */
        s++;

    /* Number of terms up to the previous diagonal (s-1). */
    seq_t tPrev = ((s - 2) * (s - 1)) / 2;

    /* Index inside the current diagonal. */
    seq_t idx = dn - tPrev - 1;            /* 0‑based */
    seq_t i   = idx + 1;                   /* first coordinate */
    seq_t j   = s - i;                     /* second coordinate */

    /* Final value: 4 * i * j. */
    return (seq_t)4 * i * j;
}
