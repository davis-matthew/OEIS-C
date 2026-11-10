/*
 * A072094  Permutation of natural numbers induced by the automorphism
 *          cdr_robr_cdr! acting on the parenthesizations encoded by A014486.
 * https://oeis.org/A072094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank (in A014486 order) of the Dyck word f(w), where w =
 *            A014486(n) read as a binary tree. Writing w = 1 A 0 B (tree (A .
 *            B)): f(empty) = empty; if A is empty, f(w) = 1 B 0; if A = 1 P 0
 *            Q, f(w) = 1 P 0 f(1 Q 0 f(B)) 0-free concatenation, i.e. (P .
 *            f(Q . f(B)))
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

static uint64_t A072094_cnt[66][68];
static int A072094_ready = 0;

static void A072094_init(void)
{
    int r, h;
    if (A072094_ready) return;
    memset(A072094_cnt, 0, sizeof(A072094_cnt));
    A072094_cnt[0][0] = 1;
    for (r = 1; r <= 64; r++) {
        for (h = 0; h <= 65; h++) {
            uint64_t v = A072094_cnt[r - 1][h + 1];
            if (h > 0) v += A072094_cnt[r - 1][h - 1];
            A072094_cnt[r][h] = v;
        }
    }
    A072094_ready = 1;
}

/* index of matching '0' for the '1' at w[0] */
static int A072094_match(const char *w)
{
    int d = 0, i = 0;
    for (;; i++) {
        if (w[i] == '1') d++;
        else d--;
        if (d == 0) return i;
    }
}

/* apply the automorphism to the Dyck word w of length len, result in out */
static int A072094_f(const char *w, int len, char *out)
{
    int m, o, k, lenA, lenB, lenP, lenQ, l1, l2, l3;
    char fb[72], t2[72], t3[72];
    const char *A, *B, *P, *Q;
    if (len == 0) return 0;
    m = A072094_match(w);
    A = w + 1;
    lenA = m - 1;
    B = w + m + 1;
    lenB = len - m - 1;
    if (lenA == 0) {
        out[0] = '1';
        memcpy(out + 1, B, (size_t)lenB);
        out[1 + lenB] = '0';
        return lenB + 2;
    }
    k = A072094_match(A);
    P = A + 1;
    lenP = k - 1;
    Q = A + k + 1;
    lenQ = lenA - k - 1;
    l1 = A072094_f(B, lenB, fb);
    t2[0] = '1';
    memcpy(t2 + 1, Q, (size_t)lenQ);
    t2[1 + lenQ] = '0';
    memcpy(t2 + lenQ + 2, fb, (size_t)l1);
    l2 = lenQ + 2 + l1;
    l3 = A072094_f(t2, l2, t3);
    o = 0;
    out[o++] = '1';
    memcpy(out + o, P, (size_t)lenP);
    o += lenP;
    out[o++] = '0';
    memcpy(out + o, t3, (size_t)l3);
    o += l3;
    return o;
}

seq_t a(idx_t n)
{
    char w[72], res[72];
    uint64_t r = (uint64_t)n, base = 0, rank = 0;
    int k = 0, len, i, h, rl;

    A072094_init();
    while (k < 32 && r >= A072094_cnt[2 * k][0]) {
        r -= A072094_cnt[2 * k][0];
        base += A072094_cnt[2 * k][0];
        k++;
    }
    len = 2 * k;
    /* unrank */
    h = 0;
    for (i = 0; i < len; i++) {
        int rem = len - i;
        if (h > 0) {
            uint64_t c = A072094_cnt[rem - 1][h - 1];
            if (r < c) {
                w[i] = '0';
                h--;
                continue;
            }
            r -= c;
        }
        w[i] = '1';
        h++;
    }
    rl = A072094_f(w, len, res);
    /* rank */
    h = 0;
    for (i = 0; i < rl; i++) {
        int rem = rl - i;
        if (res[i] == '1') {
            if (h > 0) rank += A072094_cnt[rem - 1][h - 1];
            h++;
        } else {
            h--;
        }
    }
    return (seq_t)(base + rank);
}
