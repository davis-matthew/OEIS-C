/*
 * A001167  Smallest natural number requiring n words in English (as spoken in
 *          England).
 * https://oeis.org/A001167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(1..11) found by searching n < 200000 and counting words in
 *            British English ("and" after hundred, "one thousand and one");
 *            for n >= 12, a(n) = 1000*a(n-6) + 121, since each extra
 *            three-digit group "one hundred and twenty-one <scale>" adds 6
 *            words
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   a001167_count, a001167_group, a001167_w2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001167_count.h"
#include "../utils/a001167_group.h"
#include "../utils/a001167_w2.h"

seq_t a(idx_t n) {
    static unsigned base[12];
    static int ready = 0;
    if (!ready) {
        unsigned m;
        int i;
        for (i = 0; i < 12; i++) base[i] = 0;
        for (m = 1; m < 200000; m++) {
            int c = a001167_count(m);
            if (c >= 1 && c <= 11 && base[c] == 0) base[c] = m;
        }
        ready = 1;
    }
    if (n <= 11) return (seq_t)base[n];
    idx_t r = n;
    idx_t steps = 0;
    while (r > 11) { r -= 6; steps++; }
    seq_t v = (seq_t)base[r];
    idx_t i;
    for (i = 0; i < steps; i++) v = v * 1000 + 121;
    return v;
}
