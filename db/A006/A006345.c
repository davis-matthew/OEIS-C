/*
 * A006345  Linus sequence: a(n) "breaks the pattern" by avoiding the longest
 *          doubled suffix.
 * https://oeis.org/A006345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   greedy construction: a(1) = 1; a(n) is whichever of 1,2 gives
 *            the shorter longest suffix of the form ss in a(1..n) (choose 1
 *            on a tie); repeated suffixes are found with a Z-function on the
 *            reversed word
 * Range:     u8 n <= 9736, u16 n <= 9736, u32 n <= 9736, u64 n <= 9736,
 *            u128 n <= 9736, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static unsigned char *lin_seq = NULL;
static idx_t lin_len = 0, lin_cap = 0;
static long long *lin_z = NULL;
static unsigned char *lin_r = NULL;
static idx_t lin_zcap = 0;

/* length of longest ss suffix of lin_seq[0..m-1] followed by c */
static idx_t lin_longest(idx_t m, unsigned char c) {
    idx_t L = m + 1, i, best = 0;
    long long l = 0, r = 0;
    lin_r[0] = c;
    for (i = 1; i < L; i++) lin_r[i] = lin_seq[m - i];
    lin_z[0] = (long long)L;
    for (i = 1; i < L; i++) {
        long long zi = 0;
        if ((long long)i < r) {
            zi = r - (long long)i;
            if (lin_z[i - l] < zi) zi = lin_z[i - l];
        }
        while ((idx_t)(i + zi) < L && lin_r[zi] == lin_r[i + zi]) zi++;
        lin_z[i] = zi;
        if ((long long)i + zi > r) { l = (long long)i; r = (long long)i + zi; }
        if (2 * i <= L && (idx_t)zi >= i) best = i;
    }
    return best;
}

seq_t a(idx_t n) {
    if (n > lin_len) {
        if (n > lin_cap) {
            idx_t nc = lin_cap ? lin_cap : 64;
            while (nc < n) nc *= 2;
            lin_seq = (unsigned char *)realloc(lin_seq, nc);
            lin_cap = nc;
        }
        if (n + 1 > lin_zcap) {
            idx_t nc = lin_cap + 1;
            lin_z = (long long *)realloc(lin_z, nc * sizeof(long long));
            lin_r = (unsigned char *)realloc(lin_r, nc);
            lin_zcap = nc;
        }
        if (lin_len == 0) { lin_seq[0] = 1; lin_len = 1; }
        while (lin_len < n) {
            idx_t l1 = lin_longest(lin_len, 1);
            idx_t l2 = lin_longest(lin_len, 2);
            lin_seq[lin_len] = (l1 <= l2) ? 1 : 2;
            lin_len++;
        }
    }
    return (seq_t)lin_seq[n - 1];
}
