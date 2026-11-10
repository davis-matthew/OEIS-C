/*
 * A071653  Permutation of natural numbers induced by reranking plane binary
 *          trees given in the standard lexicographic order (A014486) with an
 *          "arithmetic global ranking algorithm", using the bivariate form of
 *          A001477 as the packing bijection N x N -> N.
 * https://oeis.org/A071653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of the binary tree encoded by Dyck word A014486(n),
 *            where rank(empty) = 0 and rank(node(L,R)) =
 *            A001477-packing(rank(L),rank(R)) + 1 = (x+y)*(x+y+1)/2 + y + 1
 *            with x = rank(L), y = rank(R); the Dyck word is read as 1 L 0 R
 * Range:     u8 n <= 22, u16 n <= 64, u32 n <= 196, u64 n <= 625,
 *            u128 n <= 2055, big any n
 * Validated: standard, pass, 80 terms through n = 2052 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 A071653_u128;

#define A071653_MAXL 96

static A071653_u128 A071653_C[48];
static A071653_u128 A071653_F[A071653_MAXL + 2][A071653_MAXL + 2];
static int A071653_init = 0;

static void A071653_setup(void)
{
    if (A071653_init) return;
    A071653_C[0] = 1;
    for (int i = 0; i < 46; i++)
        A071653_C[i + 1] = A071653_C[i] * (A071653_u128)(2 * (2 * i + 1)) / (A071653_u128)(i + 2);
    /* F[r][b]: number of ways to complete with r remaining bits from balance b */
    for (int r = 0; r <= A071653_MAXL; r++)
        for (int b = 0; b <= A071653_MAXL; b++) {
            if (r == 0) { A071653_F[r][b] = (b == 0) ? 1 : 0; continue; }
            A071653_u128 v = 0;
            if (b + 1 <= A071653_MAXL) v += A071653_F[r - 1][b + 1];
            if (b > 0) v += A071653_F[r - 1][b - 1];
            A071653_F[r][b] = v;
        }
    A071653_init = 1;
}

static seq_t A071653_parse(const unsigned char *bits, int len, int *pos)
{
    if (*pos >= len || bits[*pos] == 0) return (seq_t)0;
    (*pos)++;                       /* the 1 */
    seq_t x = A071653_parse(bits, len, pos);
    (*pos)++;                       /* the matching 0 */
    seq_t y = A071653_parse(bits, len, pos);
    seq_t s = x + y;
    return s * (s + 1) / 2 + y + 1;
}

seq_t a(idx_t n)
{
    A071653_setup();
    if (n == 0) return (seq_t)0;
    A071653_u128 j = (A071653_u128)(n - 1);
    int m = 1;
    while (j >= A071653_C[m]) {
        j -= A071653_C[m];
        m++;
    }
    int L = 2 * m;
    unsigned char bits[A071653_MAXL];
    bits[0] = 1;
    int bal = 1;
    for (int p = 1; p < L; p++) {
        int rem = L - p - 1;
        if (bal > 0) {
            A071653_u128 cnt = A071653_F[rem][bal - 1];
            if (j < cnt) { bits[p] = 0; bal--; continue; }
            j -= cnt;
        }
        bits[p] = 1;
        bal++;
    }
    int pos = 0;
    return A071653_parse(bits, L, &pos);
}
