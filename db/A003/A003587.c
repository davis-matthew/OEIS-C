/*
 * A003587  Roman numerals with 1 letter, in numerical order; then those with
 *          2 letters, etc.
 * https://oeis.org/A003587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, look
 * Formula:   list 1..3999 sorted by the number of letters in their Roman
 *            numerals (ties broken by value); a(n) is the n-th entry. A digit
 *            d takes {0,1,2,3,2,1,2,3,4,2}[d] letters, thousands take d
 *            letters (M)
 * Range:     u8 n <= 5, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   a003587_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a003587_len.h"

seq_t a(idx_t n) {
    static int order[3999];
    static int done = 0;

    if (!done) {
        int cnt = 0;
        int L, v;
        for (L = 1; L <= 15; L++) {
            for (v = 1; v <= 3999; v++) {
                if (a003587_len(v) == L) {
                    order[cnt++] = v;
                }
            }
        }
        done = 1;
    }

    if (n < 1 || n > 3999) return (seq_t)0;
    return (seq_t)order[(long long)(n - 1)];
}
