/*
 * A075821  List of possible last two digits (leading zeros omitted) of
 *          perfect powers.
 * https://oeis.org/A075821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  full, fini, nonn, base
 * Formula:   a(n) is the n‑th smallest integer < 100 that is NOT congruent to
 *            2 modulo 4 and NOT divisible by 5 unless it is also divisible by
 *            25
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Example sequence (offset 1): 0, 1, 3, 4, 7, 8, 9, 11, 12, 13, 16, 17, 19, 21, 23, 24,
   25, 27, 28, 29, 31, 32, 33, 36, … (total 64 terms). */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Array of allowed residues, computed once. */
    static seq_t allowed[100];
    static idx_t len = 0;

    if (len == 0) {
        idx_t i;
        for (i = 0; i < 100; ++i) {
            bool bad1 = (i % 4 == 2);                    /* 2 mod 4      */
            bool bad2 = (i % 5 == 0 && i % 25 != 0);    /* div 5 but not 25 */
            if (!bad1 && !bad2) {
                allowed[len++] = (seq_t)i;
            }
        }
    }

    if (n == 0) {
        return (seq_t)0;  /* undefined for n < 1, return 0 as safety */
    }

    idx_t idx = n - 1;          /* shift to 0‑based index */
    if (idx < len) {
        return allowed[idx];
    }
    return (seq_t)0;            /* out of range → return 0 as sentinel */
}
