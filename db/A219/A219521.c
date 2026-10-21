/*
 * A219521  Number of 4Xn arrays of the minimum value of corresponding
 *          elements and their horizontal, diagonal or antidiagonal neighbors
 *          in a random, but sorted with lexicographically nondecreasing rows
 *          and columns, 0..1 4Xn array.
 * https://oeis.org/A219521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^8 - 12 n^7 + 490 n^6 - 4704 n^5 + 10969 n^4 + 554652
 *            n^3 - 6966660 n^2 + 36282384 n - 70842240) / 40320 for n ≥ 10.
 *            For 1 ≤ n ≤ 9 the values are 5,9,22,68,166,380,821,1700,3374
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 60, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t _a_small[9] = {
    (seq_t)5, (seq_t)9, (seq_t)22, (seq_t)68, (seq_t)166,
    (seq_t)380, (seq_t)821, (seq_t)1700, (seq_t)3374
};

seq_t a(idx_t n)
{
    /* Small n: use table. */
    if (n <= 9) return _a_small[n - 1];

    /* Use 128‑bit arithmetic for the intermediate computations
       (gcc/clang support unsigned __int128). */
    unsigned __int128 N = n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;
    unsigned __int128 n5 = n4 * N;
    unsigned __int128 n6 = n5 * N;
    unsigned __int128 n7 = n6 * N;
    unsigned __int128 n8 = n7 * N;

    unsigned __int128 num = n8
        - 12u * n7
        + 490u * n6
        - 4704u * n5
        + 10969u * n4
        + 554652u * n3
        - 6966660u * n2
        + 36282384u * N
        - 70842240u;

    /* The division is exact; cast back to seq_t (may be 8/16/32/64/128
       bits or big‑int class). */
    return (seq_t)(num / 40320u);
}
