/*
 * A207046  Number of (n+1)X5 0..2 arrays with every 2X3 or 3X2 subblock
 *          having exactly one clockwise edge increases.
 * https://oeis.org/A207046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 35, u64 n <= 83, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-3) for n>12.  Initial terms:
   a(1)=1416, a(2)=1182, a(3)=1272, a(4)=2244, a(5)=3408, a(6)=4836,
   a(7)=9144, a(8)=13950, a(9)=19752, a(10)=36912, a(11)=55944, a(12)=79032
*/
seq_t a(idx_t n)
{
    static const seq_t init[12] = {
        1416, 1182, 1272, 2244, 3408, 4836,
        9144, 13950, 19752, 36912, 55944, 79032
    };

    if (n <= 12) {
        return init[n - 1];
    }

    /* Determine the largest index <=12 with the same remainder modulo 3 */
    idx_t rem = (n - 1) % 3;
    idx_t base_index;
    if (rem == 0)          base_index = 10;
    else if (rem == 1)     base_index = 11;
    else                   base_index = 12;   /* rem == 2 */

    seq_t val = init[base_index - 1];
    idx_t exp = (n - base_index) / 3;
    while (exp--) {
        val *= 4;
    }
    return val;
}
