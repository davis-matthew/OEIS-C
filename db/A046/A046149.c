/*
 * A046149  Smallest n-digit number with maximal multiplicative persistence
 *          A014553.
 * https://oeis.org/A046149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A046149 – smallest n‑digit number with maximal multiplicative persistence.
 *
 *  For a given n, let d be the largest persistence value whose minimal record
 *  number has digit length <= n.  Let L = len[d] be that length.
 *  The answer is:
 *
 *      a(n) = (10^(n-L) - 1)/9 * 10^L + base[d]
 *
 *  where base[d] is the known minimal number achieving persistence d.
 *  For n < len[d] the formula reduces to base[d] itself.
 *
 *  Example:
 *      n = 16 -> d = 17, L = 15
 *      a(16) = (10^(1)-1)/9 * 10^15 + 277777788888899
 *            = 1 * 10^15 + 277777788888899
 *            = 1277777788888899
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Known minimal number for each persistence d (A014553). */
    static const seq_t base[] = {
        (seq_t)0,                 /* d = 0  (length 1) */
        (seq_t)10,                /* d = 1  (length 2) */
        (seq_t)25,                /* d = 2  (length 2) */
        (seq_t)39,                /* d = 3  (length 2) */
        (seq_t)77,                /* d = 4  (length 2) */
        (seq_t)679,               /* d = 5  (length 3) */
        (seq_t)6788,              /* d = 6  (length 4) */
        (seq_t)68889,             /* d = 7  (length 5) */
        (seq_t)168889,            /* d = 8  (length 6) */
        (seq_t)2677889,           /* d = 9  (length 7) */
        (seq_t)26888999,          /* d = 10 (length 8) */
        (seq_t)126888999,         /* d = 11 (length 9) */
        (seq_t)3778888999,        /* d = 12 (length 10) */
        (seq_t)13778888999,       /* d = 13 (length 11) */
        (seq_t)113778888999,      /* d = 14 (length 12) */
        (seq_t)1113778888999,     /* d = 15 (length 13) */
        (seq_t)11113778888999,    /* d = 16 (length 14) */
        (seq_t)277777788888899ULL /* d = 17 (length 15) */
    };

    /* Digit lengths of the numbers above. */
    static const idx_t len[] = {
        1, 2, 2, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    };

    /* Find the largest persistence d whose minimal record has length <= n. */
    idx_t d = 0;
    for (idx_t i = 17; i != (idx_t)(-1); --i) {
        if (len[i] <= n) {
            d = i;
            break;
        }
    }

    /* Shift (number of leading '1' digits) needed to reach n digits. */
    idx_t shift = n - len[d];

    /* Compute the prefix of ones: (10^shift - 1) / 9  (zero if shift = 0). */
    seq_t prefix = (shift == 0) ? (seq_t)0
                                : ((pow10(shift) - (seq_t)1) / (seq_t)9);

    /* 10^len[d] used to position base[d] in the n-digit number. */
    seq_t powL = pow10(len[d]);

    return prefix * powL + base[d];
}
