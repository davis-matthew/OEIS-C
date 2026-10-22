/*
 * A007005  Number of characters in the French spelling of n, including spaces
 *          and hyphens.
 * https://oeis.org/A007005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy, nice
 * Formula:   a(n) = number of characters in the traditional French spelling
 *            of n, built from parts: units, 10-19, tens (soixante-dix..,
 *            quatre-vingt..), cent(s), mille, million(s), milliard(s)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int u_len[] = {4, 2, 4, 5, 6, 4, 3, 4, 4, 4};  /* zéro..neuf */
static const int t_len[] = {3, 4, 5, 6, 8, 6, 5, 8, 8, 8};  /* dix..dix-neuf */
static const int d_len[] = {5, 6, 8, 9, 8};  /* vingt, trente, quarante, cinquante, soixante */

/* length of spelling of 1 <= n <= 99 */
static int fr_below100(int n) {
    int tens = n / 10, ones = n % 10;
    if (n < 10) return u_len[n];
    if (n < 20) return t_len[n - 10];
    if (tens < 7) {
        int r = d_len[tens - 2];
        if (ones == 0) return r;
        if (ones == 1) return r + 6;          /* " et un" */
        return r + 1 + u_len[ones];           /* "-X" */
    }
    if (tens == 7) {
        if (ones == 1) return 8 + 4 + 4;      /* soixante et onze */
        return 8 + 1 + t_len[ones];           /* soixante-dix.. */
    }
    if (tens == 8) {
        if (ones == 0) return 13;             /* quatre-vingts */
        return 12 + 1 + u_len[ones];          /* quatre-vingt-X */
    }
    return 12 + 1 + t_len[ones];              /* quatre-vingt-dix.. */
}

/* length of spelling of 1 <= n <= 999; if before_mille, plural s of
   "cents"/"quatre-vingts" is dropped */
static int fr_below1000(int n, int before_mille) {
    int h = n / 100, r = n % 100, len = 0;
    if (h == 0) {
        len = fr_below100(r);
        if (before_mille && r == 80) len--;
        return len;
    }
    if (h == 1) len = 4;                      /* cent */
    else {
        len = u_len[h] + 1 + 4;               /* X cent */
        if (r == 0 && !before_mille) len++;   /* cents */
    }
    if (r == 0) return len;
    len += 1 + fr_below100(r);
    if (before_mille && r == 80) len--;
    return len;
}

static int fr_len(uint64_t n) {
    int len = 0, parts = 0;
    if (n == 0) return 4;
    uint64_t big = n / 1000000000ULL;
    int mil = (int)(n / 1000000 % 1000);
    int th = (int)(n / 1000 % 1000);
    int low = (int)(n % 1000);
    if (big) {
        if (big == 1) len += 2 + 1 + 8;       /* un milliard */
        else len += fr_len(big) + 1 + 9;      /* X milliards */
        parts++;
    }
    if (mil) {
        if (parts) len++;
        if (mil == 1) len += 2 + 1 + 7;       /* un million */
        else len += fr_below1000(mil, 0) + 1 + 8;  /* X millions */
        parts++;
    }
    if (th) {
        if (parts) len++;
        if (th == 1) len += 5;                /* mille */
        else len += fr_below1000(th, 1) + 1 + 5;
        parts++;
    }
    if (low) {
        if (parts) len++;
        len += fr_below1000(low, 0);
    }
    return len;
}

seq_t a(idx_t n) {
    return (seq_t)fr_len((uint64_t)n);
}
