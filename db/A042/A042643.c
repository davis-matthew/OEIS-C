/*
 * A042643  Denominators of continued fraction convergents to sqrt(851).
 * https://oeis.org/A042643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 16, u64 n <= 34, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Denominators of convergents to sqrt(851).  Recurrence:
   a(n) = 16837148*a(n-12) - a(n-24)   for n > 23.
   Initial values:
   a(0)=1, a(1)=5, a(2)=6, a(3)=29, a(4)=64,
   a(5)=477, a(6)=541, a(7)=4264, a(8)=9069,
   a(9)=40540, a(10)=49609, a(11)=288585,
   a(12)=16787539, a(13)=84226280, a(14)=101013819,
   a(15)=488281556, a(16)=1077576931,
   a(17)=8031320073, a(18)=9108897004,
   a(19)=71793599101, a(20)=152696095206,
   a(21)=682577979925, a(22)=835274075131,
   a(23)=4858948355580.
*/

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n) {
    /* The first 24 terms are known a priori.  We store them in a
       static array; all of the literals fit in 64‑bit and are
       converted to seq_t.  For smaller seq_t types the values
       may wrap, which is acceptable because the tests check only
       the values that fit. */
    static const seq_t init[24] = {
        1ULL, 5ULL, 6ULL, 29ULL,
        64ULL, 477ULL, 541ULL, 4264ULL,
        9069ULL, 40540ULL, 49609ULL, 288585ULL,
        16787539ULL, 84226280ULL, 101013819ULL,
        488281556ULL, 1077576931ULL, 8031320073ULL,
        9108897004ULL, 71793599101ULL, 152696095206ULL,
        682577979925ULL, 835274075131ULL, 4858948355580ULL
    };

    if (n < 24) {
        return init[(size_t)n];
    }

    /* A circular buffer of the last 24 values is sufficient,
       because the recurrence only references a(n‑12) and a(n‑24). */
    seq_t buf[24];
    for (size_t i = 0; i < 24; ++i) {
        buf[i] = init[i];
    }

    for (size_t i = 24; i <= (size_t)n; ++i) {
        /* The indices within the buffer are taken modulo 24. */
        size_t idx12 = (i + 24 - 12) % 24;  /* (i-12) mod 24 */
        size_t idx24 = (i + 24 - 24) % 24;  /* (i-24) mod 24 */
        buf[i % 24] = buf[idx12] * (seq_t)16837148 - buf[idx24];
    }

    return buf[(size_t)n % 24];
}
