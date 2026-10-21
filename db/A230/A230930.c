/*
 * A230930  Number of black-square subarrays of (n+2) X (3+2) 0..3 arrays
 *          x(i,j) with each element diagonally or antidiagonally next to at
 *          least one element with value (x(i,j)+1) mod 4, no adjacent
 *          elements equal, and upper left element zero.
 * https://oeis.org/A230930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-2) + 3*a(n-4) - 10*a(n-6) + 24*a(n-8) - 16*a(n-10)
 *            Initial values: a(1)=8, a(2)=16, a(3)=102, a(4)=232, a(5)=1682,
 *            a(6)=3768, a(7)=27106, a(8)=60824, a(9)=437930, a(10)=982552
 *            (The recurrence holds for n ≥ 11.)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t init[10] = {
        8,   16,   102,   232,   1682,
        3768, 27106, 60824, 437930, 982552
    };
    if (n <= 10) {
        /* IDX_T is unsigned, n-1 is safe when n ≥ 1 */
        return init[n - 1];
    }

    /* Buffer holds the last ten terms:
       buf[0] = a(i-10), buf[1] = a(i-9), … , buf[9] = a(i-1)   */
    seq_t buf[10];
    for (int i = 0; i < 10; ++i)
        buf[i] = init[i];

    /* Compute a(i) for i = 11 … n */
    for (idx_t i = 11; i <= n; ++i) {
        /* a(i-2) = buf[8], a(i-4) = buf[6], a(i-6) = buf[4],
           a(i-8) = buf[2], a(i-10) = buf[0] */
        seq_t new_val = 16 * buf[8]
                      + 3 * buf[6]
                      - 10 * buf[4]
                      + 24 * buf[2]
                      - 16 * buf[0];

        /* Shift buffer left by one: drop a(i-10) */
        for (int j = 0; j < 9; ++j)
            buf[j] = buf[j + 1];
        buf[9] = new_val;  /* buf[9] now holds a(i) */
    }

    /* After the loop, buf[9] contains a(n) */
    return buf[9];
}
