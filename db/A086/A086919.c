/*
 * A086919  Smallest number whose absolute value of the successive digit
 *          difference is the n-th palindrome, or -1 if no such number exists.
 * https://oeis.org/A086919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = A327270(A002113(n)) (smallest number with
 *            digit‑difference palindrome)
 * Range:     i8 n <= 11, i16 n <= 146, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   count_pal_len, pal_digits, power10_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_pal_len.h"
#include "../utils/pal_digits.h"
#include "../utils/power10_int.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)-1;               /* sequence starts at 1 */

    uint64_t idx = (uint64_t)(n - 1);           /* 0‑based palindrome index */
    int pal[50], L;
    pal_digits(idx, pal, &L);                   /* L = length of palindrome */

    int diff_len = L;                           /* differences count */

    /* DP: possible[pos][digit]  */
    bool possible[65][10];                      /* 65 > max diff length (<= 60) */
    for (int d = 0; d < 10; ++d) possible[diff_len][d] = true;

    for (int pos = diff_len - 1; pos >= 0; --pos) {
        int p = pal[pos];
        for (int d = 0; d < 10; ++d) {
            bool ok = false;
            int t = d + p;
            if (t <= 9 && possible[pos + 1][t]) ok = true;
            t = d - p;
            if (!ok && t >= 0 && possible[pos + 1][t]) ok = true;
            possible[pos][d] = ok;
        }
    }

    /* choose smallest leading digit >=1 that leads to a solution */
    int start = -1;
    for (int d = 1; d <= 9; ++d) {
        if (possible[0][d]) { start = d; break; }
    }
    if (start == -1) return (seq_t)-1;          /* no solution */

    uint64_t value = (uint64_t)start;
    int curr = start;
    for (int pos = 0; pos < diff_len; ++pos) {
        int p = pal[pos];
        int cand1 = curr - p;
        int cand2 = curr + p;
        int next = -1;
        if (cand1 >= 0 && cand1 <= 9 && possible[pos + 1][cand1])
            next = cand1;
        if (cand2 >= 0 && cand2 <= 9 && possible[pos + 1][cand2]) {
            if (next == -1 || cand2 < next) next = cand2;
        }
        if (next == -1) return (seq_t)-1;       /* safety */
        value = value * 10ULL + (uint64_t)next;
        curr = next;
    }
    return (seq_t)value;
}
