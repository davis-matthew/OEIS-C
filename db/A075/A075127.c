/*
 * A075127  Safe perfect powers: perfect powers n such that (n-1)/2 is also a
 *          perfect power.
 * https://oeis.org/A075127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 35*a(n-1)-35*a(n-2)+a(n-3) for n>5 * with initial values:
 *            * a(1)=9, a(2)=243, a(3)=289, a(4)=9801, a(5)=332929. * This
 *            recurrence was conjectured by Colin Barker and is used here * to
 *            compute the sequence for any n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial terms for n = 1 … 5 */
    static const seq_t init[6] = {0, 9, 243, 289, 9801, 332929};

    if (n <= 5)
        return init[n];

    /* a[i-1], a[i-2], a[i-3] for the current iteration i */
    seq_t a1 = init[5];   /* a[i-1] when i = 6 */
    seq_t a2 = init[4];   /* a[i-2] when i = 6 */
    seq_t a3 = init[3];   /* a[i-3] when i = 6 */
    seq_t next;

    for (idx_t i = 6; i <= n; ++i) {
        /* Apply the recurrence relation */
        next = 35 * a1 - 35 * a2 + a3;

        /* Shift values for the next step */
        a3 = a2;
        a2 = a1;
        a1 = next;
    }

    return a1;
}
