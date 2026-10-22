/*
 * A003086  Number of self-complementary digraphs with n nodes.
 * https://oeis.org/A003086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{k in Par(m)} 2^(c(k)-p(k)) / z(k), where m =
 *            floor(n/2), z(k) = Prod_j j^t_j * t_j! (k has t_j parts equal to
 *            j, p(k) parts in all), and c(k) = Sum_i (2k_i - 1) + 2*Sum_{i<l}
 *            gcd(2k_i, 2k_l) + (n odd ? 2p(k) : 0). This is Burnside's lemma
 *            applied to permutation-complementation: only p...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   a3086_gcd, a3086_pow2mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a3086_gcd.h"
#include "../utils/a3086_pow2mod.h"

static int a3086_isfixed, a3086_W;
static int a3086_odd;
static idx_t *a3086_js, *a3086_ts;
static int a3086_k;
static uint64_t a3086_D, a3086_o;
static int a3086_s;
static seq_t a3086_inv, a3086_Q;
static unsigned __int128 a3086_R;
static seq_t a3086_M, a3086_tot;

static seq_t a3086_pow2w(uint64_t e)
{
    seq_t x = 1;
    if (e >= (uint64_t)a3086_W) return (seq_t)0;
    x <<= e;
    return x;
}

static void a3086_leaf(uint64_t e, uint64_t zf, const seq_t *zb)
{
    if (!a3086_isfixed) {
        seq_t t = a3086_M / *zb;
        t <<= e;
        a3086_tot += t;
        return;
    }
    uint64_t N = a3086_D / zf;
    if (e >= (uint64_t)a3086_s) {
        uint64_t E = e - (uint64_t)a3086_s;
        uint64_t r = (uint64_t)((unsigned __int128)(N % a3086_o) * a3086_pow2mod(E, a3086_o) % a3086_o);
        seq_t X = (seq_t)N;
        X *= a3086_pow2w(E);
        seq_t d = X;
        d -= (seq_t)r;
        d *= a3086_inv;
        a3086_Q += d;
        a3086_R += ((unsigned __int128)r) << a3086_s;
    } else {
        unsigned __int128 X = ((unsigned __int128)N) << e;
        a3086_Q += (seq_t)(uint64_t)(X / a3086_D);
        a3086_R += X % a3086_D;
    }
}

static void a3086_rec(idx_t rem, idx_t maxj, uint64_t e, uint64_t zf, const seq_t *zb)
{
    if (rem == 0) { a3086_leaf(e, zf, zb); return; }
    idx_t j0 = maxj < rem ? maxj : rem;
    for (idx_t j = j0; j >= 1; j--) {
        uint64_t cross = 0;
        for (int i = 0; i < a3086_k; i++)
            cross += 4 * a3086_ts[i] * a3086_gcd(j, a3086_js[i]);
        uint64_t zf2 = zf;
        seq_t zb2 = 0;
        if (!a3086_isfixed) zb2 = *zb;
        for (idx_t t = 1; t * j <= rem; t++) {
            if (a3086_isfixed) zf2 *= (uint64_t)(j * t);
            else { zb2 *= (seq_t)j; zb2 *= (seq_t)t; }
            uint64_t de = 2 * t * (j - 1) + 2 * j * t * (t - 1) + t * cross + (a3086_odd ? 2 * t : 0);
            a3086_js[a3086_k] = j;
            a3086_ts[a3086_k] = t;
            a3086_k++;
            a3086_rec(rem - t * j, j - 1, e + de, zf2, &zb2);
            a3086_k--;
        }
    }
}

seq_t a(idx_t n)
{
    {
        seq_t x = 1;
        int W = 0;
        while (x != 0 && W < 300) { x = (seq_t)(x + x); W++; }
        a3086_W = W;
        a3086_isfixed = (W < 300);
    }
    idx_t m = n / 2;
    a3086_odd = (int)(n & 1);
    if (a3086_isfixed && m > 20) return (seq_t)0;
    a3086_js = (idx_t *)malloc(sizeof(idx_t) * (size_t)(m + 1));
    a3086_ts = (idx_t *)malloc(sizeof(idx_t) * (size_t)(m + 1));
    a3086_k = 0;
    seq_t one = 1;
    seq_t result;
    if (a3086_isfixed) {
        uint64_t D = 1;
        for (idx_t i = 2; i <= m; i++) D *= (uint64_t)i;
        a3086_D = D;
        int s = 0;
        uint64_t o = D;
        while ((o & 1) == 0) { o >>= 1; s++; }
        a3086_s = s;
        a3086_o = o;
        seq_t ob = (seq_t)o, x = (seq_t)o;
        for (int it = 0; it < 8; it++) {
            seq_t t = ob;
            t *= x;
            seq_t two = 2;
            two -= t;
            x *= two;
        }
        a3086_inv = x;
        a3086_Q = 0;
        a3086_R = 0;
        a3086_rec(m, m, 0, 1, &one);
        a3086_Q += (seq_t)(uint64_t)(a3086_R / D);
        result = a3086_Q;
    } else {
        seq_t M = 1;
        for (idx_t i = 2; i <= m; i++) M *= (seq_t)i;
        a3086_M = M;
        a3086_tot = 0;
        a3086_rec(m, m, 0, 1, &one);
        result = a3086_tot / M;
    }
    free(a3086_js);
    free(a3086_ts);
    return result;
}
