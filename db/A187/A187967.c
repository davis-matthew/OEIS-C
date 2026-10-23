/*
 * A187967  a(n) = floor((n+k)*r) - floor(n*r) - floor(k*r), where r=sqrt(2),
 *          k=2.
 * https://oeis.org/A187967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+2)*sqrt(2)) - floor(n*sqrt(2)) - 2
 * Range:     u8 n <= 9965, u16 n <= 9965, u32 n <= 9965, u64 n <= 9965,
 *            u128 n <= 9965, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static const uint64_t SQRT2_NUM = 6369051672525773ULL; /* 2^53*sqrt(2) */
static const uint64_t SQRT2_DEN = 4503599627370496ULL; /* 2^53        */

seq_t a(idx_t n)
{
    /* floor(n * sqrt(2)) */
    unsigned __int128 prod_n  = (unsigned __int128)n * SQRT2_NUM;
    seq_t f_n = (seq_t)(prod_n / SQRT2_DEN);

    /* floor((n + 2) * sqrt(2)) */
    idx_t np2 = n + 2;
    unsigned __int128 prod_np2 = (unsigned __int128)np2 * SQRT2_NUM;
    seq_t f_np2 = (seq_t)(prod_np2 / SQRT2_DEN);

    return f_np2 - f_n - 2;
}
