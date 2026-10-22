/*
 * A005984  Related to recurrences over rings.
 * https://oeis.org/A005984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   for n = 2^k + j with 0 <= j < 2^k, a(n) = a(n-1) + j + 1 +
 *            r_k(j), a(0) = 0, where r_k(j) is the 0-based position of j when
 *            the k-bit numbers are sorted by number of 1-bits and then by
 *            decreasing value; so a(2^k - 1) = (4^k - 1)/3
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   popc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/popc.h"

static uint64_t C_tab[65][65];
static bool C_ready = false;

static void init_binom(void) {
    int i, j;
    if (C_ready) return;
    for (i = 0; i <= 64; i++) {
        C_tab[i][0] = 1;
        for (j = 1; j <= 64; j++)
            C_tab[i][j] = (i == 0) ? 0 : C_tab[i - 1][j - 1] + C_tab[i - 1][j];
    }
    C_ready = true;
}

/* number of x in [0, y) having exactly p one-bits */
static uint64_t count_lt(uint64_t y, int p) {
    uint64_t cnt = 0;
    int ones = 0, i;
    for (i = 63; i >= 0; i--) {
        if ((y >> i) & 1) {
            int need = p - ones;
            if (need >= 0 && need <= i) cnt += C_tab[i][need];
            ones++;
        }
    }
    return cnt;
}

/* position of j among k-bit numbers ordered by popcount, then decreasing value */
static uint64_t rank_k(uint64_t j, int k) {
    int p = popc(j), q;
    uint64_t r = 0;
    for (q = 0; q < p; q++) r += C_tab[k][q];
    r += C_tab[k][p] - count_lt(j + 1, p);
    return r;
}

seq_t a(idx_t n) {
    seq_t s = 0, p4 = 1;
    int k = 0;
    uint64_t j, m;
    init_binom();
    /* whole blocks [2^k, 2^(k+1)) contribute 4^k each */
    while (k < 63 && (((uint64_t)1 << (k + 1)) - 1) <= n) {
        s += p4;
        p4 *= 4;
        k++;
    }
    if (n < ((uint64_t)1 << k)) return s;
    m = n - ((uint64_t)1 << k) + 1;
    for (j = 0; j < m; j++)
        s += (seq_t)(j + 1 + rank_k(j, k));
    return s;
}
