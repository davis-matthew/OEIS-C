/*
 * A006440  Number of tree-rooted toroidal maps with 3 faces and n vertices
 *          and without separating loops.
 * https://oeis.org/A006440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = T3(n) + (n+3)*(2n+5)*Sum_{v=1..n} T1(v)*C(n-v+1) -
 *            2*(n+3)*Sum_{v=1..n} C(n-v)*(T2(v) + (n-v+1)*(2(n-v)+1)*T1(v)),
 *            where C(m) = Catalan numbers, T1(v) = binomial(2v+2,4)*C(v-1),
 *            T2(v) = 10*binomial(2v+4,6)*C(v-1), T3(v) =
 *            70*binomial(2v+6,8)*C(v-1) (all tree-rooted toroidal maps with
 *            1, 2...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   binom, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/grow_zero.h"

static seq_t *cat_tab = 0;
static idx_t cat_len = 0;

static void ensure_cat(idx_t len)
{
    idx_t i;
    if (len <= cat_len) return;
    cat_tab = grow_zero(cat_tab, cat_len, len);
    i = cat_len;
    if (i == 0) { cat_tab[0] = 1; i = 1; }
    for (; i < len; i++)
        cat_tab[i] = cat_tab[i - 1] * (seq_t)(2 * (2 * i - 1)) / (seq_t)(i + 1);
    cat_len = len;
}

seq_t a(idx_t n)
{
    seq_t T3, A, B, res;
    idx_t v;
    ensure_cat(n + 2);
    T3 = (seq_t)70 * binom(2 * n + 6, 8) * cat_tab[n - 1];
    A = 0;
    B = 0;
    for (v = 1; v <= n; v++) {
        idx_t h = n - v;
        seq_t t1 = binom(2 * v + 2, 4) * cat_tab[v - 1];
        seq_t t2 = (seq_t)10 * binom(2 * v + 4, 6) * cat_tab[v - 1];
        A += t1 * cat_tab[h + 1];
        B += cat_tab[h] * (t2 + (seq_t)(h + 1) * (seq_t)(2 * h + 1) * t1);
    }
    res = T3 + (seq_t)(n + 3) * (seq_t)(2 * n + 5) * A;
    res -= (seq_t)2 * (seq_t)(n + 3) * B;
    return res;
}
