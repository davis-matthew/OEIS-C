/*
 * A045004  Numbers whose base-3 representation contains exactly three 0's and
 *          no 2's.
 * https://oeis.org/A045004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with exactly three 0's and no 2's in base‑3 (OEIS A045004) */

#include <stdint.h>

seq_t a(idx_t n) {
    /* 1-indexed: a(1) = 27, a(2) = 82, … */
    if (n == 0) return 0;                 /* undefined, return 0 for safety */

    unsigned long long rank = (unsigned long long)n; /* remaining rank within all terms */

    /* Find the length L of the base‑3 representation. */
    unsigned long long L = 4;              /* minimum length is 4 */
    for (;;) {
        /* number of such numbers with length L is C(L-1,3) */
        unsigned long long m = L - 1;
        unsigned long long count = m * (m - 1) * (m - 2) / 6; /* C(m,3) */
        if (rank <= count) break;
        rank -= count;
        ++L;
    }

    unsigned long long positions = L - 1;   /* number of digits after the leading 1 */
    unsigned long long r = rank - 1;       /* zero‑based rank within this length */

    /* Unrank the lexicographic combination of the three zero positions
       within positions 1 … positions (these correspond to digits 2 … L). */
    unsigned long long z[3] = {0, 0, 0};    /* zero positions relative to positions 1…positions */
    unsigned long long rem = 3;             /* remaining zeros to place */
    unsigned long long start = 1;           /* next position that can be chosen */

    for (unsigned long long i = 0; i < 3; ++i) {
        /* The last position we may choose so that enough remain is:
           positions - rem + 1  (inclusive) */
        unsigned long long limit = positions - rem + 1;
        for (unsigned long long p = start; p <= limit; ++p) {
            unsigned long long count;
            unsigned long long remaining_after_p = positions - p;
            if (rem - 1 == 2) {
                count = remaining_after_p * (remaining_after_p - 1) / 2;
            } else if (rem - 1 == 1) {
                count = remaining_after_p;
            } else { /* rem - 1 == 0 */
                count = 1;
            }

            if (r < count) {
                z[i] = p;
                start = p + 1;
                --rem;
                break;
            } else {
                r -= count;
            }
        }
    }

    /* Build the base‑3 number from its digits. */
    seq_t value = 0;
    for (unsigned long long pos = 1; pos <= L; ++pos) {
        seq_t digit;
        if (pos == 1) {
            digit = 1;                       /* leading digit is always 1 */
        } else {
            unsigned long long dpos = pos - 1;/* position in the zero/one sequence */
            digit = (dpos == z[0] || dpos == z[1] || dpos == z[2]) ? 0 : 1;
        }
        value = value * (seq_t)3 + digit;
    }

    return value;
}
