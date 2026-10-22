/*
 * A000434  Number of permutations of [n] in which the longest increasing run
 *          has length 4.
 * https://oeis.org/A000434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P_4(n) - P_3(n), where P_k(n) is the number of
 *            permutations of [n] whose increasing runs all have length <= k.
 *            P_k(n) comes from a DP over (relative rank of the last element,
 *            length of the current run), built one element at a time using
 *            only additions. The DP state lives in static arrays a...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A434_MAXN 1500
#define A434_W (A434_MAXN + 2)

static seq_t a434_b4[2][5 * A434_W];
static seq_t a434_b3[2][4 * A434_W];
static seq_t a434_tot[A434_W];
static seq_t a434_ans[A434_MAXN + 1];
static int a434_p4 = 0, a434_p3 = 0;
static idx_t a434_done = 0;

static seq_t a434_rowtotal(const seq_t *cur, int k, idx_t i)
{
    seq_t s = 0;
    idx_t jp;
    int r;
    for (r = 1; r <= k; r++)
        for (jp = 1; jp <= i; jp++)
            s += cur[r * A434_W + jp];
    return s;
}

/* advance from permutations of length i (cur) to length i+1 (nxt) */
static void a434_step(const seq_t *cur, seq_t *nxt, int k, idx_t i)
{
    seq_t suf = 0;
    seq_t pre[8];
    idx_t jp;
    int r;
    for (r = 0; r < 8; r++) pre[r] = 0;
    for (jp = 1; jp <= i; jp++) {
        seq_t s = 0;
        for (r = 1; r <= k; r++) s += cur[r * A434_W + jp];
        a434_tot[jp] = s;
        suf += s;
    }
    for (jp = 1; jp <= i + 1; jp++) {
        nxt[1 * A434_W + jp] = suf;
        for (r = 1; r < k; r++) nxt[(r + 1) * A434_W + jp] = pre[r];
        if (jp <= i) {
            suf -= a434_tot[jp];
            for (r = 1; r <= k; r++) pre[r] += cur[r * A434_W + jp];
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    if (n > A434_MAXN) return 0;
    while (a434_done < n) {
        idx_t i = a434_done + 1;
        if (i == 1) {
            a434_b4[0][1 * A434_W + 1] = 1;
            a434_b3[0][1 * A434_W + 1] = 1;
            a434_p4 = 0;
            a434_p3 = 0;
        } else {
            a434_step(a434_b4[a434_p4], a434_b4[1 - a434_p4], 4, i - 1);
            a434_p4 = 1 - a434_p4;
            a434_step(a434_b3[a434_p3], a434_b3[1 - a434_p3], 3, i - 1);
            a434_p3 = 1 - a434_p3;
        }
        a434_ans[i] = a434_rowtotal(a434_b4[a434_p4], 4, i)
                    - a434_rowtotal(a434_b3[a434_p3], 3, i);
        a434_done = i;
    }
    return a434_ans[n];
}
