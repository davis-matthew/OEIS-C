/*
 * A118611  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+343)^2 = y^2.
 * https://oeis.org/A118611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A118611 – Nonnegative values x of solutions (x, y) to
   x^2 + (x + 343)^2 = y^2.
   Recurrence (valid for n > 14):
      a(n) = 6 * a(n-7) - a(n-14) + 686
   Initial values:
      a(1)=0,  a(2)=77,  a(3)=132,  a(4)=245,  a(5)=392,
      a(6)=585, a(7)=728, a(8)=1029, a(9)=1428, a(10)=1725,
      a(11)=2352, a(12)=3185, a(13)=4292, a(14)=5117
   The function computes each value by an iterative loop, avoiding any
   dynamic allocation that would be problematic for arbitrary-precision
   types.  It is valid C11 and C++17. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial sequence values (index 0 unused).  C++ constructors
       from integer literals handle arbitrary-precision seq_t; in
       C seq_t is a plain integer type. */
    static const seq_t init[15] = {
        0,    /* dummy for index 0            */
        0, 77, 132, 245, 392, 585, 728,
        1029, 1428, 1725, 2352, 3185, 4292, 5117
    };

    if (n <= 14)
        return init[(int)n];

    /* A small circular buffer holding the last 15 computed values
       (indices 0..14).  Index 0 corresponds to the value with
       n ≡ 0 (mod 15) when n ≥ 15. */
    seq_t buf[15];
    for (int i = 0; i < 15; ++i)
        buf[i] = init[i];

    /* Iterate from i = 15 up to the requested n. */
    for (idx_t i = 15; i <= n; ++i) {
        seq_t val = buf[(int)((i - 7) % 15)];
        val = val * 6;
        val = val - buf[(int)((i - 14) % 15)];
        val = val + 686;
        buf[(int)(i % 15)] = val;
    }

    return buf[(int)(n % 15)];
}
