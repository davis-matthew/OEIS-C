/*
 * A188212  Number of nondecreasing arrangements of 4 numbers in -(n+2)..(n+2)
 *          with sum zero.
 * https://oeis.org/A188212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A188212: Number of nondecreasing arrangements of 4 numbers in
   -(n+2)..(n+2) with sum zero.
   Computation is by a double loop over the first two values a<=b,
   then counting admissible third values z in closed form. */
#include <stdint.h>

seq_t a(idx_t n) {
    /* For offset 1, a(1)=18.  If n==0 we return 0. */
    if (n == 0) return 0;

    /* m = n+2, the half‑range of allowed numbers. */
    int64_t limit = (int64_t)n + 2;      /* m */
    seq_t count = 0;                    /* result */

    /* Iterate over sorted first two values a≤b.  Since the sum must be
       zero, a cannot be >0. */
    for (int64_t a = -limit; a <= 0; ++a) {
        for (int64_t b = a; b <= limit; ++b) {
            int64_t s = a + b;                  /* a+b */

            /* Lower bound for z:
               z must be at least b (sorted) and also satisfy
               -m - (a+b) ≤ z.                     */
            int64_t L = b;
            int64_t temp = -(limit + s);
            if (temp > L) L = temp;

            /* Upper bound for z:
               From d = -(a+b+z) ≥ z   →   z ≤ -(a+b)/2.
               Use a floor division for negative arguments.    */
            int64_t s_neg = -s;
            int64_t U;
            if (s_neg >= 0)
                U = s_neg / 2;
            else
                U = (s_neg - 1) / 2;           /* floor for negative */

            if (U > limit) U = limit;          /* also z ≤ m */

            if (L <= U) {
                /* All integer z in [L, U] are valid.               */
                count += (seq_t)(U - L + 1);
            }
        }
    }
    return count;
}
