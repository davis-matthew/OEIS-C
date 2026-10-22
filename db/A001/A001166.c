/*
 * A001166  Smallest natural number requiring n letters in English.
 * https://oeis.org/A001166
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  word, nonn
 * Formula:   constructive search, not a closed formula. A natural number (>=
 *            1) is written in British English in groups of three digits: "one
 *            hundred and one", "one thousand and one". The name ends with
 *            "and" when the last group is 1..99 and a higher group is
 *            nonzero. To find a(n), try the number of scale gro...
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdbool.h>

#define NSCALES 21
#define MAXS 1100

static const char *const u_names[] = {
    "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
    "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen",
    "sixteen", "seventeen", "eighteen", "nineteen"
};
static const char *const t_names[] = {
    "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"
};
static const char *const scale_names[NSCALES] = {
    "", "thousand", "million", "billion", "trillion", "quadrillion", "quintillion",
    "sextillion", "septillion", "octillion", "nonillion", "decillion", "undecillion",
    "duodecillion", "tredecillion", "quattuordecillion", "quindecillion",
    "sexdecillion", "septendecillion", "octodecillion", "novemdecillion"
};

static int grp_len[1000];
static unsigned char reach[NSCALES + 1][MAXS];
static bool inited = false;

static int below100(int g) {
    if (g < 20) return (int)strlen(u_names[g]);
    return (int)strlen(t_names[g / 10]) + (int)strlen(u_names[g % 10]);
}

/* letters of group g at position j (j = 0 is the last group), given that a higher group is nonzero */
static int cost_low(int j, int g) {
    if (g == 0) return 0;
    if (j == 0) return grp_len[g] + (g < 100 ? 3 : 0);
    return grp_len[g] + (int)strlen(scale_names[j]);
}

static void init_tables(void) {
    int g, j, s;
    for (g = 0; g < 1000; g++) {
        int h = g / 100, r = g % 100, c = 0;
        if (h > 0) {
            c += (int)strlen(u_names[h]) + 7;
            if (r > 0) c += 3;
        }
        c += below100(r);
        grp_len[g] = c;
    }
    memset(reach, 0, sizeof(reach));
    reach[0][0] = 1;
    for (j = 0; j < NSCALES; j++) {
        for (s = 0; s < MAXS; s++) {
            if (!reach[j][s]) continue;
            for (g = 0; g < 1000; g++) {
                int t = s + cost_low(j, g);
                if (t < MAXS) reach[j + 1][t] = 1;
            }
        }
    }
    inited = true;
}

seq_t a(idx_t n) {
    int k, g, j;
    if (!inited) init_tables();
    if (n >= (idx_t)MAXS) return (seq_t)0;
    int target = (int)n;
    for (k = 0; k < NSCALES; k++) {
        for (g = 1; g < 1000; g++) {
            int c = (k == 0) ? grp_len[g] : grp_len[g] + (int)strlen(scale_names[k]);
            int rem = target - c;
            if (rem < 0 || !reach[k][rem]) continue;
            seq_t val = (seq_t)g;
            for (j = k - 1; j >= 0; j--) {
                int h;
                for (h = 0; h < 1000; h++) {
                    int r2 = rem - cost_low(j, h);
                    if (r2 >= 0 && reach[j][r2]) {
                        rem = r2;
                        break;
                    }
                }
                val = val * 1000 + (seq_t)h;
            }
            return val;
        }
    }
    return (seq_t)0;
}
