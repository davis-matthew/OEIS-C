/*
 * A008261  Coordination sequence for quartz.
 * https://oeis.org/A008261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 143, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula
   a(0) = 1
   a(6m)  = 114 m^2 + 2            (m > 0)
   a(6m+1)= 114 m^2 + 38 m + 4
   a(6m+2)= 114 m^2 + 76 m + 14    (m > 0)
   a(6m+3)= 114 m^2 + 114 m + 30
   a(6m+4)= 114 m^2 + 152 m + 52
   a(6m+5)= 114 m^2 + 190 m + 80
*/
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    seq_t m = n / 6;
    idx_t r = n % 6;

    seq_t m2 = m * m;
    seq_t val;

    switch (r)
    {
    case 0:                 /* n = 6m, m > 0 */
        val = (seq_t)114 * m2 + (seq_t)2;
        break;
    case 1:
        val = (seq_t)114 * m2 + (seq_t)38 * m + (seq_t)4;
        break;
    case 2:
        if (n == (idx_t)2)
            val = (seq_t)12;
        else
            val = (seq_t)114 * m2 + (seq_t)76 * m + (seq_t)14;
        break;
    case 3:
        val = (seq_t)114 * m2 + (seq_t)114 * m + (seq_t)30;
        break;
    case 4:
        val = (seq_t)114 * m2 + (seq_t)152 * m + (seq_t)52;
        break;
    default:                /* r == 5 */
        val = (seq_t)114 * m2 + (seq_t)190 * m + (seq_t)80;
        break;
    }
    return val;
}
