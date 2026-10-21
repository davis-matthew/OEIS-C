/*
 * A093211  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 11.
 * https://oeis.org/A093211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A093211 – largest integer whose all n‑digit substrings are distinct
 *  and divisible by 11.
 *
 *  For n≥3 odd:
 *    a(n) = (n−1)×'9' , 0 , (n−1)×'2' , 0 , (n−1)×'9'
 *
 *  For n≥4 even:
 *    a(n) = (n−2)×'9' , 8 , 8 , (n−2)×'9' , 8
 *
 *  Special cases:
 *    a(1) = 0
 *    a(2) = 99
 *
 *  The construction uses only integer arithmetic and is O(n) per call.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)0;
    if (n == (idx_t)2) return (seq_t)99;

    idx_t k;
    seq_t val = (seq_t)0;

    if (n % (idx_t)2 == (idx_t)1) {            /* n is odd */
        k = n - (idx_t)1;                     /* k = n−1 */
        for (idx_t i = (idx_t)0; i < k; ++i)
            val = val * (seq_t)10 + (seq_t)9;
        val = val * (seq_t)10 + (seq_t)0;
        for (idx_t i = (idx_t)0; i < k; ++i)
            val = val * (seq_t)10 + (seq_t)2;
        val = val * (seq_t)10 + (seq_t)0;
        for (idx_t i = (idx_t)0; i < k; ++i)
            val = val * (seq_t)10 + (seq_t)9;
    } else {                                  /* n is even and n≥4 */
        k = n - (idx_t)2;                     /* k = n−2 */
        for (idx_t i = (idx_t)0; i < k; ++i)
            val = val * (seq_t)10 + (seq_t)9;
        val = val * (seq_t)10 + (seq_t)8;
        val = val * (seq_t)10 + (seq_t)8;
        for (idx_t i = (idx_t)0; i < k; ++i)
            val = val * (seq_t)10 + (seq_t)9;
        val = val * (seq_t)10 + (seq_t)8;
    }

    return val;
}
