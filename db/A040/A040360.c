/*
 * A040360  Continued fraction for sqrt(380).
 * https://oeis.org/A040360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction for sqrt(380) a(0) = 19 For n > 0: a(n) = 2
 *            if n is odd, 38 if n is even
 * Range:     u8 n <= 1, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    static bool init = false;
    static seq_t a0, a1, a2;
    if (!init) {
        /* D is the radicand (380) */
        const seq_t D = 380;
        /* a0 = floor(sqrt(D)) = 19 */
        a0 = 19;

        /* Compute a1 */
        seq_t m = 0;
        seq_t d = 1;
        seq_t new_m = d * a0 - m;      /* 1*19 - 0 = 19 */
        seq_t new_d = (D - new_m * new_m) / d;  /* (380-361)/1 = 19 */
        a1 = (a0 + new_m) / new_d;    /* (19+19)/19 = 2 */

        /* Compute a2 */
        m = new_m;
        d = new_d;
        new_m = d * a1 - m;           /* 19*2-19 = 19 */
        new_d = (D - new_m * new_m) / d;  /* (380-361)/19 = 1 */
        a2 = (a0 + new_m) / new_d;    /* (19+19)/1 = 38 */

        init = true;
    }
    if (n == 0) return a0;
    return (n & 1) ? a1 : a2;
}
