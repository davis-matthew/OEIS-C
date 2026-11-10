/*
 * A069774  Permutation of natural numbers induced by the automorphism
 *          RoblDownCar_et_SwapInv! acting on the parenthesizations encoded by
 *          A014486.
 * https://oeis.org/A069774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014486-rank of F(T_n), where T_n is the n-th binary tree
 *            (Dyck word) in A014486 order and F is defined recursively:
 *            F(nil) = nil; F([nil,B]) = [B,nil]; F([X,C]) = [p,[q,C]] where
 *            [p,q] = F(X), for X != nil. Words are unranked and ranked with a
 *            ballot-number table
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

typedef unsigned __int128 dy_u128;

static dy_u128 dy_W[84][85];
static int dy_ready = 0;

static void dy_init(void)
{
    int r, h;
    if (dy_ready) return;
    memset(dy_W, 0, sizeof(dy_W));
    dy_W[0][0] = 1;
    for (r = 1; r <= 82; r++) {
        for (h = 0; h <= 82; h++) {
            dy_u128 v = dy_W[r - 1][h + 1];
            if (h > 0) v += dy_W[r - 1][h - 1];
            dy_W[r][h] = v;
        }
    }
    dy_ready = 1;
}

/* index of the '0' matching the '1' at position s */
static int dy_match(const char *w, int s)
{
    int d = 0, i;
    for (i = s; ; i++) {
        d += w[i] ? 1 : -1;
        if (d == 0) return i;
    }
}

/* apply the automorphism to the tree encoded by w[0..len), write to out */
static void dy_F(const char *w, int len, char *out)
{
    int m, xl, cl, pos = 0;
    const char *X, *c;
    if (len == 0) return;
    m = dy_match(w, 0);
    X = w + 1;
    xl = m - 1;
    c = w + m + 1;
    cl = len - m - 1;
    if (xl == 0) {
        out[pos++] = 1;
        if (cl > 0) memcpy(out + pos, c, (size_t)cl);
        pos += cl;
        out[pos++] = 0;
        return;
    }
    {
        char y[200];
        int mm, pl, ql;
        dy_F(X, xl, y);
        mm = dy_match(y, 0);
        pl = mm - 1;
        ql = xl - mm - 1;
        out[pos++] = 1;
        if (pl > 0) memcpy(out + pos, y + 1, (size_t)pl);
        pos += pl;
        out[pos++] = 0;
        out[pos++] = 1;
        if (ql > 0) memcpy(out + pos, y + mm + 1, (size_t)ql);
        pos += ql;
        out[pos++] = 0;
        if (cl > 0) memcpy(out + pos, c, (size_t)cl);
    }
}

seq_t a(idx_t n)
{
    char w[200], o[200];
    int k = 0, len, i, h;
    dy_u128 r = (dy_u128)n;
    dy_u128 base = 0;

    dy_init();
    for (k = 0; ; k++) {
        dy_u128 c = dy_W[2 * k][0];
        if (r < c) break;
        r -= c;
        base += c;
    }
    len = 2 * k;
    if (len == 0) return (seq_t)0;

    /* unrank */
    h = 0;
    for (i = 0; i < len; i++) {
        int rem = len - i - 1;
        if (h > 0) {
            dy_u128 cnt = dy_W[rem][h - 1];
            if (r < cnt) { w[i] = 0; h--; continue; }
            r -= cnt;
        }
        w[i] = 1;
        h++;
    }

    dy_F(w, len, o);

    /* rank */
    {
        dy_u128 rr = 0;
        h = 0;
        for (i = 0; i < len; i++) {
            int rem = len - i - 1;
            if (o[i]) {
                if (h > 0) rr += dy_W[rem][h - 1];
                h++;
            } else {
                h--;
            }
        }
        rr += base;
        return (seq_t)(idx_t)rr;
    }
}
