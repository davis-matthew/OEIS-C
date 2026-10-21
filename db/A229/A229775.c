/*
 * A229775  Number of 3 X 3 0..n arrays with rows in lexicographically
 *          nondecreasing order and columns in nonincreasing order.
 * https://oeis.org/A229775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the 10‑term linear recurrence of OEIS A229775 a(n)
 *            = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) + 252*a(n-5) -
 *            210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) - a(n-10) for
 *            n>10 initial terms: a(1)=20, a(2)=322, a(3)=3232, a(4)=21331,
 *            a(5)=103222, a(6)=397460, a(7)=1287672, a(8)...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 18, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Base values a(1) .. a(10) */
    static const seq_t base[10] = {
        (seq_t)20,      (seq_t)322,      (seq_t)3232,
        (seq_t)21331,   (seq_t)103222,   (seq_t)397460,
        (seq_t)1287672, (seq_t)3647349,  (seq_t)9281272,
        (seq_t)21642038
    };

    if (n <= 10) return base[n - 1];

    /* Variables for the last ten values of the sequence:
       a1 = a(n-10), a2 = a(n-9), …, a10 = a(n-1)  */
    seq_t a1 = base[0], a2 = base[1], a3 = base[2], a4 = base[3],
          a5 = base[4], a6 = base[5], a7 = base[6], a8 = base[7],
          a9 = base[8], a10 = base[9], next;

    /* Compute up to the requested n */
    for (idx_t i = 11; i <= n; ++i) {
        /* Parts with positive coefficients */
        seq_t part1 = a10 * (seq_t)10    + a8 * (seq_t)120
                     + a6 * (seq_t)252   + a4 * (seq_t)120
                     + a2 * (seq_t)10;

        /* Parts with negative coefficients */
        seq_t part2 = a9 * (seq_t)45     + a7 * (seq_t)210
                     + a5 * (seq_t)210   + a3 * (seq_t)45
                     + a1;

        next = part1 - part2;            /* guaranteed non‑negative */

        /* Shift values for the next iteration */
        a1 = a2; a2 = a3; a3 = a4; a4 = a5;
        a5 = a6; a6 = a7; a7 = a8; a8 = a9;
        a9 = a10; a10 = next;
    }
    return a10;
}
