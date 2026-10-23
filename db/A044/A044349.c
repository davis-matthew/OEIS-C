/*
 * A044349  Numbers n such that string 1,7 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Sequence A044349:  Numbers n such that the decimal string "17" occurs in n
 * but does not occur in n-1.  The sequence begins 17, 117, 170, 217, …   */
/* Term formula: for n ≥ 1, a(n) is the n‑th integer k≥1 with
 *   contains17(k) == true  and  contains17(k‑1) == false. */

/* Helper: Check whether a positive seq_t value contains the decimal
 * substring “17”.  The test is performed from least‑significant to
 * most‑significant digit.  All arithmetic uses only integer operators
 * allowed for seq_t. */
static bool contains17(seq_t v)
{
    const seq_t TEN = 10;              /* fits into seq_t for any build */
    if (v == 0) return false;          /* 0 has no digits */
    seq_t prev = v % TEN;              /* last (units) digit */
    v = v / TEN;
    while (v > 0)
    {
        seq_t cur = v % TEN;           /* next more significant digit */
        if (cur == 1 && prev == 7)
            return true;
        prev = cur;
        v = v / TEN;
    }
    return false;
}

/* Main sequence function.  It scans integers starting from 1 until the
 * n‑th qualifying term is found.  The function works for any build
 * because it only uses integer operations and no dynamic allocation. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for offset > 0 */

    idx_t cur = 1;                     /* integer being inspected  */
    idx_t count = 0;                   /* number of terms found so far */

    while (count < n)
    {
        bool cur_has = contains17(cur);
        bool prev_has = (cur > 1) ? contains17(cur - 1) : false;

        if (cur_has && !prev_has)
        {
            ++count;
            if (count == n)
            {
                seq_t result = cur;   /* conversion to seq_t */
                return result;
            }
        }
        ++cur;
    }
    /* Should never reach here, but keep compiler happy. */
    return 0;
}
