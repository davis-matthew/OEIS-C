/*
 * A181326  Number of columns with an odd sum in all 2-compositions of n. A
 *          2-composition of n is a nonnegative matrix with two rows, such
 *          that each column has at least one nonzero entry and whose entries
 *          sum up to n.
 * https://oeis.org/A181326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A181326: 
   a(n) = Number of columns with an odd sum among all 2‑compositions of n.
   Recurrence (order 6):
     a(0)=0, a(1)=2, a(2)=8, a(3)=40, a(4)=168, a(5)=696
     for n≥6:
       a(n) = 6·a(n‑1) – 5·a(n‑2) – 16·a(n‑3) + 8·a(n‑4) + 8·a(n‑5) – 4·a(n‑6)
*/
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0 … 5 */
    static const seq_t init[6] = { (seq_t)0, (seq_t)2, (seq_t)8,
                                   (seq_t)40, (seq_t)168, (seq_t)696 };

    if (n < 6) return init[n];

    /* arr[0] = a(n‑6), … , arr[5] = a(n‑1) */
    seq_t arr[6];
    for (size_t i = 0; i < 6; ++i) arr[i] = init[i];

    for (idx_t i = 6; i <= n; ++i)
    {
        /* Positive contributions */
        seq_t pos = arr[5] * (seq_t)6  +  /* 6·a(n‑1) */
                    arr[2] * (seq_t)8  +  /* 8·a(n‑4) */
                    arr[1] * (seq_t)8;   /* 8·a(n‑5) */

        /* Negative contributions */
        seq_t neg = arr[4] * (seq_t)5  +  /* 5·a(n‑2) */
                    arr[3] * (seq_t)16 +  /* 16·a(n‑3) */
                    arr[0] * (seq_t)4;   /* 4·a(n‑6) */

        /* a(n) = pos – neg (non‑negative by definition) */
        seq_t next = pos - neg;

        /* Shift the array for the next iteration */
        for (size_t j = 0; j < 5; ++j) arr[j] = arr[j + 1];
        arr[5] = next;
    }

    return arr[5];
}
