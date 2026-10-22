/*
 * A006969  Number of characters in French ordinal numbers.
 * https://oeis.org/A006969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = A007005(n) + e, where e = 4, except a(1) = 7 ("premier"),
 *            e = 5 when the French name of n ends in "cinq", and e = 3 when
 *            it ends in "-e" or in a plural "-s". The code builds the French
 *            cardinal name, then adds the length of the ordinal suffix
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

static const char *a6969_small[17] = {
    "zero", "un", "deux", "trois", "quatre", "cinq", "six", "sept", "huit",
    "neuf", "dix", "onze", "douze", "treize", "quatorze", "quinze", "seize"
};
static const char *a6969_tens[10] = {
    "", "dix", "vingt", "trente", "quarante", "cinquante", "soixante",
    "soixante", "quatre-vingt", "quatre-vingt"
};
static const char *a6969_big[7] = {
    "", "mille", "million", "milliard", "billion", "billiard", "trillion"
};

static void a6969_below100(char *s, int x, int pl)
{
    if (x < 17) { strcat(s, a6969_small[x]); return; }
    if (x < 20) { strcat(s, "dix-"); strcat(s, a6969_small[x - 10]); return; }
    {
        int t = x / 10, u = x % 10;
        if (t == 7 || t == 9) {
            strcat(s, a6969_tens[t]);
            if (t == 7 && u == 1) { strcat(s, "-et-onze"); return; }
            strcat(s, "-");
            a6969_below100(s, 10 + u, pl);
            return;
        }
        strcat(s, a6969_tens[t]);
        if (u == 0) {
            if (t == 8 && pl) strcat(s, "s");
            return;
        }
        if (u == 1 && t != 8) { strcat(s, "-et-un"); return; }
        strcat(s, "-");
        strcat(s, a6969_small[u]);
    }
}

static void a6969_below1000(char *s, int x, int pl)
{
    int h = x / 100, r = x % 100;
    if (h == 0) { a6969_below100(s, r, pl); return; }
    if (h > 1) { strcat(s, a6969_small[h]); strcat(s, "-"); }
    strcat(s, "cent");
    if (r == 0) {
        if (h > 1 && pl) strcat(s, "s");
        return;
    }
    strcat(s, "-");
    a6969_below100(s, r, pl);
}

seq_t a(idx_t n)
{
    char s[512];
    int grp[8];
    int ng = 0, i;
    uint64_t m = (uint64_t)n;
    size_t L;
    if (n == 1) return (seq_t)7;
    while (m > 0) { grp[ng++] = (int)(m % 1000); m /= 1000; }
    s[0] = 0;
    for (i = ng - 1; i >= 0; i--) {
        int g = grp[i];
        if (g == 0) continue;
        if (s[0]) strcat(s, "-");
        if (i == 0) {
            a6969_below1000(s, g, 1);
        } else if (i == 1) {
            if (g != 1) { a6969_below1000(s, g, 0); strcat(s, "-"); }
            strcat(s, "mille");
        } else {
            a6969_below1000(s, g, 1);
            strcat(s, "-");
            strcat(s, a6969_big[i]);
            if (g > 1) strcat(s, "s");
        }
    }
    L = strlen(s);
    if (s[L - 1] == 'e') return (seq_t)(L + 3);
    if (L >= 2 && s[L - 1] == 's' && (s[L - 2] == 't' || s[L - 2] == 'n'))
        return (seq_t)(L + 3);
    if (L >= 4 && strcmp(s + L - 4, "cinq") == 0) return (seq_t)(L + 5);
    return (seq_t)(L + 4);
}
