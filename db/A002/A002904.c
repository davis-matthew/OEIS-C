/*
 * A002904  Delete all letters except c, d, i, l, m, v, x from the US English
 *          name of n, then read as Roman numeral if possible, otherwise 0.
 * https://oeis.org/A002904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   write n in US English words (no "and"), keep only the letters
 *            c,d,i,l,m,v,x, and read the result as a Roman numeral. a(n) =
 *            its value if it is the standard (canonical) Roman form of that
 *            value, otherwise 0. a(n) = 0 for n >= 10^6 ("ll")
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

static void a002904_cat(char *buf, size_t *pos, const char *w) {
    while (*w) buf[(*pos)++] = *w++;
    buf[*pos] = '\0';
}

static void a002904_canon(int v, char *out) {
    static const int vals[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    static const char *syms[] = {"m", "cm", "d", "cd", "c", "xc", "l", "xl", "x", "ix",
                                 "v", "iv", "i"};
    size_t pos = 0;
    int k;
    out[0] = '\0';
    for (k = 0; k < 13; k++) {
        while (v >= vals[k]) {
            a002904_cat(out, &pos, syms[k]);
            v -= vals[k];
        }
    }
}

static int a002904_val(char c) {
    switch (c) {
    case 'i': return 1;
    case 'v': return 5;
    case 'x': return 10;
    case 'l': return 50;
    case 'c': return 100;
    case 'd': return 500;
    case 'm': return 1000;
    }
    return 0;
}

static const char *a002904_ones[] = {"", "one", "two", "three", "four", "five", "six",
                                     "seven", "eight", "nine"};
static const char *a002904_teens[] = {"ten", "eleven", "twelve", "thirteen", "fourteen",
                                      "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
static const char *a002904_tens[] = {"", "", "twenty", "thirty", "forty", "fifty", "sixty",
                                     "seventy", "eighty", "ninety"};

static void a002904_below1000(unsigned n, char *buf, size_t *pos) {
    if (n >= 100) {
        a002904_cat(buf, pos, a002904_ones[n / 100]);
        a002904_cat(buf, pos, "hundred");
        n %= 100;
    }
    if (n >= 20) {
        a002904_cat(buf, pos, a002904_tens[n / 10]);
        n %= 10;
        a002904_cat(buf, pos, a002904_ones[n]);
    } else if (n >= 10) {
        a002904_cat(buf, pos, a002904_teens[n - 10]);
    } else {
        a002904_cat(buf, pos, a002904_ones[n]);
    }
}

seq_t a(idx_t n) {
    char eng[128], rom[128], can[64];
    size_t pos = 0, rl = 0, i;
    int v = 0;
    unsigned m;

    if (n >= 1000000) return 0;
    m = (unsigned)n;
    eng[0] = '\0';
    if (m >= 1000) {
        a002904_below1000(m / 1000, eng, &pos);
        a002904_cat(eng, &pos, "thousand");
        m %= 1000;
    }
    a002904_below1000(m, eng, &pos);

    for (i = 0; eng[i]; i++)
        if (a002904_val(eng[i])) rom[rl++] = eng[i];
    rom[rl] = '\0';
    if (rl == 0) return 0;

    for (i = 0; i < rl; i++) {
        int cur = a002904_val(rom[i]);
        int nxt = (i + 1 < rl) ? a002904_val(rom[i + 1]) : 0;
        if (nxt > cur) v -= cur; else v += cur;
    }
    if (v <= 0 || v >= 4000) return 0;
    a002904_canon(v, can);
    if (strcmp(can, rom) != 0) return 0;
    return (seq_t)v;
}
