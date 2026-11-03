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
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int a001167_w2(unsigned r) {
    if (r == 0) return 0;
    if (r < 20 || r % 10 == 0) return 1;
    return 2;
}

static int a001167_group(unsigned x) {
    unsigned h = x / 100, r = x % 100;
    int c = 0;
    if (h) c += 2;
    if (h && r) c += 1;
    return c + a001167_w2(r);
}

static int a001167_count(unsigned n) {
    unsigned T = n / 1000, U = n % 1000;
    int c = 0;
    if (T) c += a001167_group(T) + 1;
    c += a001167_group(U);
    if (T && U > 0 && U < 100) c += 1;
    return c;
}

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
