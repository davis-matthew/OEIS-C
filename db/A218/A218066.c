/*
 * A218066  Number of 3Xn arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random 0..1 3Xn array.
 * https://oeis.org/A218066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence from OEIS A218066:
 * a(n) = 2*a(n‑1) + 11*a(n‑2) - 7*a(n‑3) - 34*a(n‑4) + 11*a(n‑5)
 *        + 49*a(n‑6) - 14*a(n‑7) - 36*a(n‑8) + 7*a(n‑9) + a(n‑10)
 *        + 8*a(n‑11) + 2*a(n‑12) - 13*a(n‑13) + 2*a(n‑14) + 4*a(n‑15)
 * for n > 18, with initial values
 * a(1)=8, a(2)=16, a(3)=74, a(4)=260, a(5)=1046, a(6)=3974,
 * a(7)=15578, a(8)=60242, a(9)=234468, a(10)=910060, a(11)=3536762,
 * a(12)=13737348, a(13)=53371468, a(14)=207332364, a(15)=805465810,
 * a(16)=3129084596, a(17)=12156035112, a(18)=47224200410 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* first 18 terms */
    static const seq_t init[18] = {
        8ULL, 16ULL, 74ULL, 260ULL, 1046ULL, 3974ULL,
        15578ULL, 60242ULL, 234468ULL, 910060ULL, 3536762ULL,
        13737348ULL, 53371468ULL, 207332364ULL, 805465810ULL,
        3129084596ULL, 12156035112ULL, 47224200410ULL
    };
    if (n <= 18) return init[n-1];

    /* last 15 values: a(4) … a(18) (indices 3 … 17 in init) */
    seq_t last[15];
    for (size_t i = 0; i < 15; ++i) last[i] = init[3 + i];

    seq_t term = 0;
    for (idx_t k = 19; k <= n; ++k) {
        seq_t pos = 0;
        pos += (seq_t)2 * last[14]; /* a(k-1) */
        pos += (seq_t)11 * last[13]; /* a(k-2) */
        pos += (seq_t)11 * last[10]; /* a(k-5) */
        pos += (seq_t)49 * last[9];  /* a(k-6) */
        pos += (seq_t)7  * last[6];  /* a(k-9) */
        pos += (seq_t)1  * last[5];  /* a(k-10) */
        pos += (seq_t)8  * last[4];  /* a(k-11) */
        pos += (seq_t)2  * last[3];  /* a(k-12) */
        pos += (seq_t)2  * last[1];  /* a(k-14) */
        pos += (seq_t)4  * last[0];  /* a(k-15) */

        seq_t neg = 0;
        neg += (seq_t)7  * last[12]; /* a(k-3) */
        neg += (seq_t)34 * last[11]; /* a(k-4) */
        neg += (seq_t)14 * last[8];  /* a(k-7) */
        neg += (seq_t)36 * last[7];  /* a(k-8) */
        neg += (seq_t)13 * last[2];  /* a(k-13) */

        term = pos - neg;

        /* shift array left: discard oldest (a(k-15)) */
        for (size_t i = 0; i < 14; ++i) last[i] = last[i + 1];
        last[14] = term;
    }
    return term;
}
