/*
 * A065230  Fill a triangular array by rows by writing numbers 1 up to b(0), 1
 *          up to b(1), etc., where b(n) are the hexagonal numbers. The first
 *          elements of the rows form a(n).
 * https://oeis.org/A065230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A065230 - first element of each row of a triangle whose rows have
   lengths equal to the hexagonal numbers H(k)=k*(2k-1).
   a(0) = 1.
   For n ≥ 1   let temp = a(n‑1)+n.
   If temp exceeds the next unused hexagonal number H(k), set
        a(n) = temp – H(k)      and advance to the next hexagonal number H(k+1).
   Otherwise a(n) = temp.
   This implements the definition “Fill a triangular array by rows by writing
   numbers 1 up to b(0), 1 up to b(1), …, where b(n) are the hexagonal numbers;
   the first elements of the rows form a(n). */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t val = (seq_t)1;          /* a(0)             */
    idx_t  k   = (idx_t)1;         /* index of next H(k)*/
    seq_t  next_hex = (seq_t)k * (((seq_t)2 * k) - (seq_t)1); /* H(1)=1 */

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        seq_t temp = val + (seq_t)i;        /* a(i‑1)+i          */
        if (temp > next_hex) {              /* crossing a hex     */
            val = temp - next_hex;          /* subtract it        */
            ++k;                            /* advance to next hex*/
            next_hex = (seq_t)k * (((seq_t)2 * k) - (seq_t)1);
        } else {
            val = temp;                     /* no subtraction     */
        }
    }
    return val;
}
