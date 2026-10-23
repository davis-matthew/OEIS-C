/*
 * A188371  a(n) = [n*r] - [k*r] - [n*r-k*r], where r=1/sqrt(2), k=6, [
 *          ]=floor.
 * https://oeis.org/A188371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n / sqrt(2)) - floor(6 / sqrt(2)) - floor((n-6) /
 *            sqrt(2))
 * Range:     u8 n <= 11, u16 n <= 133, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses integer arithmetic only; the floor of x / sqrt(2) is computed
   as floor( sqrt( floor(x*x / 2) ) ) which is exact for all integer x. */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Compute floor(n / sqrt(2)) for a non‑negative integer n. */
static seq_t floor_div_sqrt2(idx_t n)
{
    seq_t sq = (seq_t)n;
    sq = sq * sq;          /* n^2 */
    seq_t half = sq / 2;   /* floor(n^2 / 2)  */
    return floor_sqrt(half); /* floor( sqrt( floor(n^2/2) ) ) == floor(n / sqrt(2)) */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t fk;
    static int init = 0;
    if (!init) {
        fk = floor_div_sqrt2(6); /* constant floor(6 / sqrt(2)) */
        init = 1;
    }

    seq_t fn = floor_div_sqrt2(n);

    if (n < 6) {
        /* For n < 6, floor((n-6)/sqrt(2)) = -( floor((6-n)/sqrt(2)) + 1 ) */
        idx_t pos = 6 - n;
        seq_t p = (seq_t)pos;
        seq_t sq = p * p;      /* (6-n)^2 */
        seq_t half = sq / 2;   /* floor((6-n)^2 / 2)  */
        seq_t t = floor_sqrt(half);      /* floor((6-n)/sqrt(2)) */
        return fn - fk + (t + 1);         /* fn - fk - (-(t+1)) */
    } else {
        seq_t fnk = floor_div_sqrt2(n - 6);
        return fn - fk - fnk;
    }
}
