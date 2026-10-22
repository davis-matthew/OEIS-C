/*
 * A003029  Number of unilaterally connected digraphs with n labeled nodes.
 * https://oeis.org/A003029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum over ordered set partitions (B_1,...,B_k) of [n] of
 *            Prod_i A003030(|B_i|) * Prod_i (2^(|B_i|*|B_{i+1}|) - 1) *
 *            Prod_{j>i+1} 2^(|B_i|*|B_j|). The strong components of a
 *            unilateral digraph form a chain: consecutive blocks need at
 *            least one forward arc, and other forward arcs are free. A0...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define A3029_MAXN 160

static seq_t Cb[A3029_MAXN + 1][A3029_MAXN + 1];
static seq_t dP[A3029_MAXN + 1], dQ[A3029_MAXN + 1], sc[A3029_MAXN + 1];
static seq_t F[A3029_MAXN + 1][A3029_MAXN + 1];
static seq_t Tt[A3029_MAXN + 1];
static idx_t done_n = 0;
static int wbits = -1; /* 0 means arbitrary precision */

static int seq_width_bits(void) {
    if (wbits < 0) {
        seq_t x = (seq_t)1;
        int b = 0;
        while (b < 1024 && x != 0) { x = x + x; b++; }
        wbits = (x == 0) ? b : 0;
    }
    return wbits;
}

static seq_t shl_safe(seq_t x, uint64_t s) {
    int w = seq_width_bits();
    if (w && s >= (uint64_t)w) return (seq_t)0;
    return (seq_t)(x << s);
}

static void extend(idx_t nn) {
    idx_t k, m, mp;
    if (nn == 1) Cb[0][0] = (seq_t)1;
    Cb[nn][0] = (seq_t)1;
    for (k = 1; k < nn; k++) Cb[nn][k] = Cb[nn - 1][k] + Cb[nn - 1][k - 1];
    Cb[nn][nn] = (seq_t)1;

    /* delta(nn) as P - Q */
    seq_t P = shl_safe((seq_t)1, (uint64_t)nn * (nn - 1));
    seq_t Q = (seq_t)0;
    for (k = 1; k < nn; k++) {
        uint64_t sh = (uint64_t)k * (nn - k) + (uint64_t)(nn - k) * (nn - k - 1);
        if (dQ[k] != 0) P += shl_safe(Cb[nn][k] * dQ[k], sh);
        if (dP[k] != 0) Q += shl_safe(Cb[nn][k] * dP[k], sh);
    }
    if (P >= Q) { P -= Q; Q = (seq_t)0; } else { Q -= P; P = (seq_t)0; }
    dP[nn] = P; dQ[nn] = Q;

    /* strongly connected count s(nn) */
    seq_t sP = P, sQ = Q;
    for (k = 1; k < nn; k++) {
        seq_t u = Cb[nn - 1][k - 1] * sc[k];
        if (dP[nn - k] != 0) sP += u * dP[nn - k];
        if (dQ[nn - k] != 0) sQ += u * dQ[nn - k];
    }
    sc[nn] = sP - sQ;

    /* F[nn][mp]: unilateral structures on nn nodes whose last block has size mp */
    seq_t tot = sc[nn];
    F[nn][nn] = sc[nn];
    for (mp = 1; mp < nn; mp++) {
        idx_t Np = nn - mp;
        seq_t acc = shl_safe(Tt[Np], (uint64_t)Np * mp);
        for (m = 1; m <= Np; m++)
            acc -= shl_safe(F[Np][m], (uint64_t)(Np - m) * mp);
        F[nn][mp] = Cb[nn][mp] * (sc[mp] * acc);
        tot += F[nn][mp];
    }
    Tt[nn] = tot;
}

seq_t a(idx_t n) {
    if (n < 1 || n > A3029_MAXN) return (seq_t)0;
    while (done_n < n) {
        done_n++;
        extend(done_n);
    }
    return Tt[n];
}
