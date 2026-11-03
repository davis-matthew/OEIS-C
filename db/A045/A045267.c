/*
 * A045267  Numbers whose base-5 representation contains exactly one 2 and one
 *          3.
 * https://oeis.org/A045267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search of numbers whose base‑5 representation contains
 * exactly one digit 2 and exactly one digit 3. */

#include <stdbool.h>

/* Helper: test whether the base‑5 representation of x contains exactly one
 * 2 and exactly one 3 (and any number of 0,1,4). */
static inline bool has_one2_one3(seq_t x)
{
    bool seen2 = false, seen3 = false;
    seq_t t = x;
    while (t) {
        seq_t d = t % ((seq_t)5);
        if (d == (seq_t)2) {
            if (seen2) return false;
            seen2 = true;
        } else if (d == (seq_t)3) {
            if (seen3) return false;
            seen3 = true;
        }
        t /= ((seq_t)5);
    }
    return seen2 && seen3;
}

/* OEIS A045267: numbers whose base‑5 representation contains exactly one 2
 * and one 3. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, guard against n==0 */
    idx_t found = 0;
    for (seq_t v = 1; ; ++v) {      /* iterate natural numbers */
        if (has_one2_one3(v)) {
            ++found;
            if (found == n) return v;
        }
    }
}
