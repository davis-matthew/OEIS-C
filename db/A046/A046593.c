/*
 * A046593  First denominator and then numerator of each element of the
 *          1/4-Pascal triangle (by row),excluding 1's and 2's.
 * https://oeis.org/A046593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046593: First denominator and then numerator of each element of the 1/4‑Pascal triangle (by row),
   excluding 1's and 2's.
   The triangle is generated row by row:
     Row 0: 1/1
     Row 1: 1/1 1/1
     Row 2: 1/1 1/4 1/1              (special case)
     For n ≥ 3, Row n is obtained by summing adjacent entries of Row n‑1.
   Each fraction is reduced to lowest terms.
   While traversing the triangle by rows, for every fraction f = p/q
   we output first q then p, but any number equal to 1 or 2 is omitted.
   The sequence a(n) is the n‑th number produced by this process. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

typedef struct { seq_t num; seq_t den; } Rational;

seq_t a(idx_t n) {
    idx_t produced = 0;

    /* start with row 0: {1/1} */
    Rational *prev = (Rational*)malloc(sizeof(Rational));
    prev[0].num = (seq_t)1;
    prev[0].den = (seq_t)1;
    size_t prev_len = 1;
    idx_t row = 0;

    while (1) {
        /* process the current row */
        for (size_t i = 0; i < prev_len; ++i) {
            seq_t num = prev[i].num;
            seq_t den = prev[i].den;
            if (den != (seq_t)1 && den != (seq_t)2) {
                ++produced;
                if (produced == n) {
                    free(prev);
                    return den;
                }
            }
            if (num != (seq_t)1 && num != (seq_t)2) {
                ++produced;
                if (produced == n) {
                    free(prev);
                    return num;
                }
            }
        }
        if (produced >= n) break;           /* safe guard */

        /* compute next row */
        size_t next_len = prev_len + 1;
        Rational *next = (Rational*)malloc(next_len * sizeof(Rational));
        next[0].num = (seq_t)1;
        next[0].den = (seq_t)1;
        next[next_len - 1].num = (seq_t)1;
        next[next_len - 1].den = (seq_t)1;

        if (row == 0) {
            /* Row 1: no interior entries, so just the two 1/1s */
        } else if (row == 1) {
            /* special Row 2: 1/1 1/4 1/1 */
            next[1].num = (seq_t)1;
            next[1].den = (seq_t)4;
        } else {
            /* ordinary rows: sum of adjacent entries of previous row */
            for (size_t i = 1; i + 1 < next_len; ++i) {
                Rational left = prev[i - 1];
                Rational right = prev[i];
                seq_t tmp_num = left.num * right.den + right.num * left.den;
                seq_t tmp_den = left.den * right.den;
                seq_t g = gcd(tmp_num, tmp_den);
                next[i].num = tmp_num / g;
                next[i].den = tmp_den / g;
            }
        }

        free(prev);
        prev = next;
        prev_len = next_len;
        ++row;
    }

    free(prev);
    return (seq_t)0;        /* unreachable for n ≥ 1 */
}
