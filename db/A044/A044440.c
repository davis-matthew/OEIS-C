/*
 * A044440  Numbers n such that string 2,1 occurs in the base 3 representation
 *          of n but not of n+1.
 * https://oeis.org/A044440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A044440
   Numbers n such that the base‑3 representation of n contains the
   digit sequence "21", but the base‑3 representation of n+1 does not.
*/

#include <stddef.h>
#include <stdbool.h>

/* Check if the base‑3 representation of x contains the substring 2,1.
   It scans digits from least significant to most significant,
   keeping track whether the previous digit observed was a '1'. */
static bool has21(seq_t x) {
    if (x == 0) return false;

    const seq_t three = (seq_t)3;
    bool prev_is_one = false;
    seq_t temp = x;

    while (temp > 0) {
        seq_t cur = temp % three;             /* current digit (least significant) */
        if (cur == (seq_t)2 && prev_is_one)    /* pattern 1 (previous) 2 (current) -> 21 in normal order */
            return true;
        prev_is_one = (cur == (seq_t)1);       /* remember if this digit is 1 for next iteration */
        temp /= three;                         /* move to next more significant digit */
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, so n>=1 expected */

    idx_t found = 0;
    seq_t candidate = 0;

    /* Sequentially search for numbers with the required property. */
    while (found < n) {
        ++candidate;
        if (has21(candidate) && !has21(candidate + 1)) {
            ++found;
        }
    }
    return candidate;
}
