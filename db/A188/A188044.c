/*
 * A188044  a(n) = [n*r] - [k*r] - [n*r-k*r], where r=sqrt(2), k=4, [ ]=floor.
 * https://oeis.org/A188044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(2)) - 5 - floor((n-4)*sqrt(2))
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static inline seq_t floor_sqrt2(idx_t n)
{
    /* Computes floor(n*sqrt(2)) as floor(sqrt(2*n*n)) using 128‑bit arithmetic */
    unsigned __int128 prod = (unsigned __int128)n * n * 2;
    return floor_sqrt((seq_t)prod);
}

seq_t a(idx_t n)
{
    seq_t fn = floor_sqrt2(n);        /* floor(n*sqrt2)              */

    if (n >= 4) {
        seq_t f4 = floor_sqrt2(n - 4);/* floor((n-4)*sqrt2) for n>=4 */
        return fn - (seq_t)5 - f4;
    } else {
        /* For n < 4, floor((n-4)*sqrt2) = -(floor((4-n)*sqrt2)+1) */
        seq_t fm = floor_sqrt2(4 - n);/* floor((4-n)*sqrt2)          */
        return fn + fm - (seq_t)4;
    }
}
