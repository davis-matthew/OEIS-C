/*
 * A122043  Rotations of 123, 245, 367, 489 (terms differ by 122) juxtaposed
 *          with the product of their last 2 digits. See example line.
 * https://oeis.org/A122043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = concat( r(n), ((r(n)/10)%10) * (r(n)%10) ), where r(n)
 *            cycles through the 12 rotations of {123,245,367,489}
 * Range:     u8 none, u16 n <= 7, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 12 rotations of the four 3‑digit numbers. */
    static const seq_t rot[12] = {
        123, 312, 231,   /* rotations of 123 */
        245, 524, 452,   /* rotations of 245 */
        367, 736, 673,   /* rotations of 367 */
        489, 894, 948    /* rotations of 489 */
    };

    /* Determine which rotation to use. */
    idx_t idx  = (n - 1) % 12;          /* 0‑based index */
    seq_t x    = rot[(size_t)idx];      /* the chosen 3‑digit number */

    /* Product of the last two digits. */
    seq_t tens   = (x / 10) % 10;
    seq_t ones   =  x % 10;
    seq_t prod   = tens * ones;

    /* Factor to shift the 3‑digit number left enough to concatenate. */
    seq_t factor = (prod < 10) ? 10 : 100;

    return x * factor + prod;
}
