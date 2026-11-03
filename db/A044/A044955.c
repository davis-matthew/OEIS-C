/*
 * A044955  Numbers with no two equally numerous base 6 digits.
 * https://oeis.org/A044955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers with no two equally numerous base‑6 digits
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: check if a number has distinct non‑zero digit counts in base‑6 */
static inline bool distinct_counts_base6(seq_t n)
{
    /* maximum possible digit count is the number of base‑6 digits;
       we use a small fixed array for the seen counts. */
    int count[6] = {0};
    seq_t tmp = n;
    while (tmp != 0) {
        int d = (int)(tmp % 6);  /* digit 0..5 */
        count[d] += 1;
        tmp /= 6;
    }
    bool seen[128] = {false};
    for (int i = 0; i < 6; ++i) {
        int c = count[i];
        if (c > 0) {
            if (seen[c]) return false;   /* duplicate count found */
            seen[c] = true;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cur   = 1;
    while (found < n) {
        if (distinct_counts_base6(cur))
            ++found;
        if (found == n)
            break;
        ++cur;
    }
    return cur;
}
