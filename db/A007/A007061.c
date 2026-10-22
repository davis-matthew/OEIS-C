/*
 * A007061  The Ehrenfeucht-Mycielski sequence (1,2-version): a maximally
 *          unpredictable sequence.
 * https://oeis.org/A007061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Ehrenfeucht-Mycielski rule on bits b = a-1: start 0,1.
 *            For the current word U(m), let v be the longest suffix of U(m)
 *            that also occurs in U(m-1). Take its last occurrence; the next
 *            bit is the complement of the bit after it. Then a(n) = b(n) + 1.
 *            Each step keeps the common-suffix lengths M...
 * Range:     u8 n <= 1999, u16 n <= 1999, u32 n <= 1999, u64 n <= 1999,
 *            u128 n <= 1999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned char *em_bits = NULL;
static idx_t *em_M = NULL;
static idx_t em_len = 0;
static idx_t em_cap = 0;

static void em_reserve(idx_t need) {
    if (need <= em_cap) return;
    idx_t nc = em_cap ? em_cap : 64;
    while (nc < need) nc *= 2;
    em_bits = (unsigned char *)realloc(em_bits, nc * sizeof(unsigned char));
    em_M = (idx_t *)realloc(em_M, nc * sizeof(idx_t));
    em_cap = nc;
}

static void em_extend(idx_t n) {
    if (em_len == 0) {
        em_reserve(64);
        em_bits[0] = 0;
        em_bits[1] = 1;
        em_M[0] = 0; /* common suffix of "0" and "01" */
        em_len = 2;
    }
    if (n <= em_len) return;
    em_reserve(n + 1);
    while (em_len < n) {
        idx_t m = em_len;
        /* find last e in [0, m-2] with maximal M[e] */
        idx_t best = 0, beste = m - 2;
        for (idx_t e = 0; e + 2 <= m; e++) {
            if (em_M[e] >= best) {
                best = em_M[e];
                beste = e;
            }
        }
        unsigned char nb = (unsigned char)(1 - em_bits[beste + 1]);
        em_bits[m] = nb;
        /* update common-suffix lengths for end positions 0..m-1 */
        for (idx_t e = m; e-- > 0;) {
            if (em_bits[e] == nb)
                em_M[e] = (e > 0 ? em_M[e - 1] : 0) + 1;
            else
                em_M[e] = 0;
        }
        em_len = m + 1;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    em_extend(n);
    return (seq_t)(em_bits[n - 1] + 1);
}
