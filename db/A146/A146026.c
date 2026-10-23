/*
 * A146026  Numbers that can be written from base 2 to base 9 using only the
 *          digits 0 to 3.
 * https://oeis.org/A146026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer that can be represented in every base 2
 *            to 9 using only the digits 0 to 3. The sequence starts
 *            0,1,2,3,8281,8282,8283,... For n ≤ 4 the values are n‑1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   check_bases
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is computed by enumerating numbers in base 9 with digits 0–3,
   in increasing numeric order, and selecting those that also use
   only digits 0–3 in bases 5–8.  */
#include <stdbool.h>
#include "../utils/check_bases.h"

/* Recursive enumeration of base‑9 numbers with digits 0‑3.
   Parameters:
      len  – total number of digits (most significant first)
      pos  – current position (0 … len‑1)
      val  – numeric value built so far
   The function updates global counters and stops once the target
   term has been found. */
static idx_t target_index;
static idx_t count_valid;
static seq_t  result_val;
static bool   found_flag;

static void gen_rec(int len, int pos, seq_t val)
{
    if (found_flag) return;
    if (pos == len) {
        if (val <= 3) return;                    /* skip 1,2,3 */
        if (check_bases(val)) {
            ++count_valid;
            if (count_valid == target_index) {
                result_val = val;
                found_flag = true;
            }
        }
        return;
    }
    for (int d = 0; d < 4; ++d) {                /* digits 0..3 */
        if (pos == 0 && d == 0) continue;        /* leading digit non‑zero */
        gen_rec(len, pos + 1, val * 9 + d);
    }
}

/* Public sequence function required by the build. */
seq_t a(idx_t n)
{
    /* The first four terms are trivially 0,1,2,3. */
    if (n <= 4) return (seq_t)(n - 1);

    /* For n > 4 we need the (n‑4)-th positive valid number
       greater than 3. */
    target_index  = n - 4;
    count_valid   = 0;
    found_flag    = false;
    result_val    = 0;

    int len = 1;
    while (!found_flag) {
        gen_rec(len, 0, (seq_t)0);
        ++len;
    }
    return result_val;
}
