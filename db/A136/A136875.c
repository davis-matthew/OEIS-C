/*
 * A136875  Numbers k such that k and k^2 use only the digits 0, 1, 6 and 7.
 * https://oeis.org/A136875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A136875: numbers k such that k and k^2 use only the digits 0,1,6,7.
   Construction:
   - start with 0;
   - output 10^m for m = 0,1,2,3,4,5,6,
   - for m >= 6, after outputting 10^m, also output 1077776·10^(m-6).
   This interleaving produces the sequence: 0,1,10,100,1000,10000,100000,1000000,
   1077776,10000000,10777760,100000000,107777600, … */
seq_t a(idx_t n)
{
    if (n == 1) return 0;                /* a(1) = 0 */
    idx_t count = 1;                     /* count of values emitted so far (0 included) */
    seq_t pow10 = 1;                     /* current 10^m */
    seq_t c_val = 1077776;               /* current 1077776·10^(m-6) for m >= 6 */

    for (idx_t m = 0; ; ++m) {
        /* emit 10^m */
        ++count;
        if (count == n) return pow10;

        /* after 10^m, if m >= 6 also emit corresponding 1077776*10^(m-6) */
        if (m >= 6) {
            ++count;
            if (count == n) return c_val;
            c_val *= 10;                 /* prepare for next m */
        }

        /* advance to next power of ten for 10^(m+1) */
        pow10 *= 10;
    }
}
