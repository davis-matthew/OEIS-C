/*
 * A166924  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A166924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166924
   a(0) = 1, a(1)=9, a(2)=72, a(3)=576, a(4)=4608, a(5)=36864,
   a(6)=294912, a(7)=2359296, a(8)=18874368, a(9)=150994944,
   a(10)=1207959552, a(11)=9663676416, a(12)=77309411328,
   a(13)=618475290588
   Recurrence (for n ≥ 14):
        a(n) = 7 * (a(n-1)+…+a(n-12)) – 28 * a(n-13)            */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* base values 0 .. 13 */
    static const seq_t base[14] = {
        (seq_t)1ULL,
        (seq_t)9ULL,
        (seq_t)72ULL,
        (seq_t)576ULL,
        (seq_t)4608ULL,
        (seq_t)36864ULL,
        (seq_t)294912ULL,
        (seq_t)2359296ULL,
        (seq_t)18874368ULL,
        (seq_t)150994944ULL,
        (seq_t)1207959552ULL,
        (seq_t)9663676416ULL,
        (seq_t)77309411328ULL,
        (seq_t)618475290588ULL
    };

    /* if the requested index is within the base range, return directly */
    if (n < 14)
        return base[n];

    /* keep the last thirteen terms a(n-13) … a(n-1) in a fixed‑size array */
    seq_t last[13];

    /* initialise with a(1) … a(13) */
    for (idx_t i = 0; i < 13; ++i)
        last[i] = base[i + 1];

    const seq_t coeff7 = (seq_t)7;
    const seq_t coeff28 = (seq_t)28;

    /* compute terms from n = 14 up to the requested n */
    for (idx_t i = 14; i <= n; ++i)
    {
        /* sum of a(i-1) … a(i-12)  which are last[1] … last[12] */
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j < 13; ++j)
            sum += last[j];

        /* recurrence */
        seq_t next = coeff7 * sum - coeff28 * last[0];

        /* shift the window: discard a(i-13), shift left, append next */
        for (idx_t j = 0; j < 12; ++j)
            last[j] = last[j + 1];
        last[12] = next;
    }

    /* after the loop, last[12] holds a(n) */
    return last[12];
}
