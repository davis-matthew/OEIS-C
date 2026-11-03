/*
 * A031466  Numbers whose base-4 representation has one fewer 0 than 3's.
 * https://oeis.org/A031466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer whose base‑4 representation contains one
 *            more 3 than 0. Equivalently, count_4(n,3) = count_4(n,0)+1
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 8800 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Test whether x has one more 3 than 0 in base‑4. */
static int has_one_more_three_than_zero(seq_t x)
{
    seq_t tmp = x;
    int zeros  = 0;
    int threes = 0;
    const seq_t ZERO = 0;
    while (tmp != ZERO) {
        int d = (int)(tmp % 4);   /* digit 0–3 */
        if (d == 0) ++zeros;
        else if (d == 3) ++threes;
        tmp /= 4;
    }
    return threes == zeros + 1;
}

/* Return the n‑th term (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for index 0 */

    seq_t cur   = 1;
    idx_t found = 0;

    while (1) {
        if (has_one_more_three_than_zero(cur)) {
            ++found;
            if (found == n) return cur;
        }
        ++cur;
    }
}
