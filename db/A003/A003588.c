/*
 * A003588  Roman numerals with 1 letter, in alphabetical order; then those
 *          with 2 letters, etc.
 * https://oeis.org/A003588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, look
 * Formula:   search - write each k = 1..3999 as a standard Roman numeral,
 *            then sort by length and alphabetically within each length; a(n)
 *            is the n-th numeral's value
 * Range:     u8 n <= 1, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   A3588_roman
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/A3588_roman.h"

static char A3588_str[4000][16];
static int A3588_order[3999];
static int A3588_ready = 0;

static int A3588_cmp(const void *p, const void *q) {
    int x = *(const int *)p, y = *(const int *)q;
    size_t lx = strlen(A3588_str[x]), ly = strlen(A3588_str[y]);
    if (lx != ly) return lx < ly ? -1 : 1;
    return strcmp(A3588_str[x], A3588_str[y]);
}

seq_t a(idx_t n) {
    int k;
    if (!A3588_ready) {
        for (k = 1; k <= 3999; k++) {
            A3588_roman(k, A3588_str[k]);
            A3588_order[k - 1] = k;
        }
        qsort(A3588_order, 3999, sizeof(int), A3588_cmp);
        A3588_ready = 1;
    }
    if (n < 1 || n > 3999) return (seq_t)0;
    return (seq_t)A3588_order[(long long)n - 1];
}
