/*
 * A009956  Coordination sequence for FeS2-Pyrite, S position.
 * https://oeis.org/A009956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is given by a(n) = a(n-2) + 2*a(n-4) - 2*a(n-6) - a(n-8) +
 *            a(n-10) (n > 12) with initial values a(0)=1, a(1)=4, a(2)=18,
 *            a(3)=37, a(4)=70, a(5)=105, a(6)=169, a(7)=206, a(8)=292,
 *            a(9)=343, a(10)=469, a(11)=513, a(12)=664
 * Range:     u8 n <= 7, u16 n <= 119, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Initial values up to a(12) */
    static const seq_t init[13] = {
        1, 4, 18, 37, 70, 105,
        169, 206, 292, 343, 469, 513, 664
    };

    if (n <= 12)
        return init[n];

    /* Buffer of 13 elements to hold values needed for the recurrence */
    seq_t buf[13];
    for (idx_t i = 0; i <= 12; ++i)
        buf[i] = init[i];

    /* Compute a(n) iteratively using the recurrence */
    for (idx_t i = 13; i <= n; ++i) {
        seq_t val = buf[(i - 2) % 13] + 2 * buf[(i - 4) % 13]
                    - 2 * buf[(i - 6) % 13] - buf[(i - 8) % 13]
                    + buf[(i - 10) % 13];
        buf[i % 13] = val;
    }

    return buf[n % 13];
}
