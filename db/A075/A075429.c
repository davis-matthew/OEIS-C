/*
 * A075429  First integer reached when starting with 1+2/(n*(n+1)) and
 *          iterating the map x -> x*ceiling(x) A075428(n) times, or -1 if no
 *          integer is ever reached.
 * https://oeis.org/A075429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = the first integer reached when starting from
 *   x0 = 1 + 2/(n·(n+1)) and iterating the map
 *   x -> x * ceil(x), for A075428(n) steps.
 *   If no integer is ever reached it returns -1.
 *
 *   In other words:
 *     let d = n·(n+1)
 *     let p = d + 2   (p/q = 1 + 2/d)
 *     repeat:
 *        if p % d == 0 return p/d
 *        k = ceil(p/d) = (p + d – 1) / d
 *        p = p * k
 *        (stop after a safe number of iterations and return -1)
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)-1;          /* offset: a(1) is first term */

    /* denominator d = n*(n+1) */
    seq_t nd = (seq_t)n;
    seq_t d  = nd * (nd + 1);

    /* numerator p = d + 2  =>  (n*(n+1)+2)/(n*(n+1)) */
    seq_t p = d + 2;

    /* iterate until p is divisible by d or we give up */
    const idx_t MAX_STEPS = 100000;
    for (idx_t step = 0; step < MAX_STEPS; ++step) {
        if (p % d == 0) {
            return p / d;                 /* first integer reached */
        }
        /* ceil(p/d) = (p + d - 1) / d  (integer division) */
        seq_t k = (p + d - 1) / d;
        p = p * k;
    }
    /* no integer reached within the limit */
    return (seq_t)-1;
}
