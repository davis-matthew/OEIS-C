/*
 * A160662  Numbers that have a "e" in the middle of their names.
 * https://oeis.org/A160662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, less
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A160662 – numbers with an 'e' in the middle of their
   English name.  Standard American spelling, no hyphens and no "and".
   The function enumerates integers until the n‑th term that satisfies
   the condition is found. */

/* ---------- helpers for word conversion ---------- */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static const char *const units[] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};

static const char *const tens[] = {
    "", "", "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
};

static const char *const scales[] = {
    "", "thousand", "million", "billion", "trillion",
    "quadrillion", "quintillion", "sextillion", "septillion",
    "octillion", "nonillion", "decillion"
};

static void sub_to_words(unsigned int sub, char *out)
{
    /* sub is in [1,999] */
    if (sub < 20) {
        strcpy(out, units[sub]);
        return;
    }
    if (sub < 100) {
        unsigned int t = sub / 10;
        unsigned int o = sub % 10;
        strcpy(out, tens[t]);
        if (o) strcat(out, units[o]);
    } else {
        unsigned int h = sub / 100;
        unsigned int rem = sub % 100;
        strcpy(out, units[h]);
        strcat(out, "hundred");
        if (rem) {
            char tmp[128];
            sub_to_words(rem, tmp);
            strcat(out, tmp);
        }
    }
}

static void number_to_words(uint64_t n, char *out)
{
    if (n == 0) {
        strcpy(out, "zero");
        return;
    }
    /* up to 15 groups (quintillion range) – plenty for 64‑bit numbers */
    char seg[15][128];
    int segcnt = 0;
    unsigned int group = 0;
    while (n) {
        unsigned int sub = n % 1000;
        if (sub) {
            char tmp[128];
            sub_to_words(sub, tmp);
            if (group >= 1) {
                strcat(tmp, scales[group]);
            }
            strcpy(seg[segcnt++], tmp);
        }
        n /= 1000;
        group++;
    }
    out[0] = '\0';
    for (int i = segcnt - 1; i >= 0; i--) {
        strcat(out, seg[i]);
    }
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    /* enumerate integers until nth match reached                       */
    idx_t count = 0;
    uint64_t candidate = 0;

    while (1) {
        candidate++;                       /* start from 1                       */
        char buf[512];
        number_to_words(candidate, buf);
        size_t len = strlen(buf);
        if (len & 1) {                       /* odd length, has single middle    */
            char mid = buf[len / 2];
            if (mid == 'e' || mid == 'E') {
                count++;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
    }
}
