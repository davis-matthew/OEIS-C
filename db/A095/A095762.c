/*
 * A095762  Numbers whose name in English contains an "h".
 * https://oeis.org/A095762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose name in English contains an "h" */
/* Implementation: brute‑force search using simple linguistic rules */
#include <stdbool.h>

static bool contains_h(idx_t n)
{
    if (n >= 1000) return true;           /* contains "thousand" */
    if (n >= 100)  return true;           /* contains "hundred"  */

    if (n < 20) {
        return (n == 3 || n == 8 || n == 13 || n == 18);
    }

    /* 20 ≤ n ≤ 99 */
    idx_t tens  = n / 10;
    idx_t units = n % 10;

    if (tens == 3 || tens == 8) return true;   /* "thirty", "eighty" */
    if (units == 3 || units == 8) return true;   /* "three", "eight" */
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;

    for (idx_t i = 1; ; ++i) {
        if (contains_h(i)) {
            ++count;
            if (count == n) {
                return (seq_t)i;
            }
        }
    }
}
