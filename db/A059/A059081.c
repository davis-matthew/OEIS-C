/*
 * A059081  Number of 5-element T_0-antichains on a labeled n-set, n=0,..,32.
 * https://oeis.org/A059081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = (1/120) * ( [32]_n - 20*[24]_n + 60*[20]_n + 20*[18]_n +
 *            10*[17]_n - 110*[16]_n - 120*[15]_n + 150*[14]_n + 120*[13]_n -
 *            240*[12]_n 20*[11]_n + 240*[10]_n 40*[9]_n - 205*[8]_n 60*[7]_n
 *            - 210*[6]_n + 210*[5]_n 50*[4]_n - 100*[3]_n 24*[2]_n ) where
 *            [k]_n = k*(k-1)*…*(k-n+1) (falling factoria...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   falling_int128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/falling_int128.h"

seq_t a(idx_t n)
{
    /* Use signed 128‑bit integer for intermediate sums.
       The final result is an unsigned integer. */
    if (n == 0) return (seq_t)0;

    static const struct {
        int coeff;
        idx_t k;
    } terms[] = {
        {  1, 32 },
        { -20, 24 },
        {  60, 20 },
        {  20, 18 },
        {  10, 17 },
        { -110, 16 },
        { -120, 15 },
        {  150, 14 },
        {  120, 13 },
        { -240, 12 },
        {   20, 11 },
        {  240, 10 },
        {   40,  9 },
        { -205,  8 },
        {   60,  7 },
        { -210,  6 },
        {  210,  5 },
        {   50,  4 },
        { -100,  3 },
        {   24,  2 }
    };

    const int m = sizeof(terms) / sizeof(terms[0]);
    __int128 sum = 0;

    for (int i = 0; i < m; ++i) {
        if (n > terms[i].k) continue;          /* falling factorial is zero */
        sum += (__int128)terms[i].coeff * falling_int128(n, terms[i].k);
    }

    /* Divide by 5! = 120, then cast to seq_t.  The division is exact. */
    return (seq_t)(sum / 120);
}
