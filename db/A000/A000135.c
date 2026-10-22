/*
 * A000135  Number of partitions into non-integral powers.
 * https://oeis.org/A000135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{nonempty finite sets S of positive integers : Sum_{x in
 *            S} x^(2/3) <= n}. Computed by search (meet in the middle): first
 *            a sorted list of subset sums over the small x, then a DFS over
 *            subsets of the large x that binary-searches the list. Each
 *            weight x^(2/3) is stored exactly as floor(2^6...
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned __int128 a135_u128;

static void a135_mulw(const uint64_t *a, int na, const uint64_t *b, int nb, uint64_t *r)
{
    int i, j;
    for (i = 0; i < na + nb; i++) r[i] = 0;
    for (i = 0; i < na; i++) {
        uint64_t carry = 0;
        for (j = 0; j < nb; j++) {
            a135_u128 t = (a135_u128)a[i] * b[j] + r[i + j] + carry;
            r[i + j] = (uint64_t)t;
            carry = (uint64_t)(t >> 64);
        }
        r[i + nb] = carry;
    }
}

/* c^3 <= x2 * 2^192 ? */
static int a135_cube_le(a135_u128 c, uint64_t x2)
{
    uint64_t a[2], sq[4], cu[6], t[6] = {0, 0, 0, 0, 0, 0};
    int i;
    a[0] = (uint64_t)c;
    a[1] = (uint64_t)(c >> 64);
    a135_mulw(a, 2, a, 2, sq);
    a135_mulw(sq, 4, a, 2, cu);
    t[3] = x2;
    for (i = 5; i >= 0; i--) {
        if (cu[i] < t[i]) return 1;
        if (cu[i] > t[i]) return 0;
    }
    return 1;
}

/* floor(2^64 * x^(2/3)) */
static a135_u128 a135_weight(uint64_t x, int top)
{
    uint64_t x2 = x * x;
    a135_u128 y = 0;
    int b;
    for (b = top; b >= 0; b--) {
        a135_u128 c = y | ((a135_u128)1 << b);
        if (a135_cube_le(c, x2)) y = c;
    }
    return y;
}

static uint64_t a135_isqrt(uint64_t v)
{
    uint64_t r = 0, bit;
    for (bit = (uint64_t)1 << 31; bit; bit >>= 1) {
        uint64_t c = r | bit;
        if ((a135_u128)c * c <= v) r = c;
    }
    return r;
}

static const a135_u128 *a135_w;
static uint64_t a135_M;
static const a135_u128 *a135_s;
static size_t a135_ns;

static size_t a135_cnt(a135_u128 R)
{
    size_t lo = 0, hi = a135_ns;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (a135_s[mid] <= R) lo = mid + 1; else hi = mid;
    }
    return lo;
}

static a135_u128 a135_dfs(uint64_t i, a135_u128 R)
{
    a135_u128 tot = a135_cnt(R);
    uint64_t j;
    for (j = i; j <= a135_M && a135_w[j] <= R; j++)
        tot += a135_dfs(j + 1, R - a135_w[j]);
    return tot;
}

seq_t a(idx_t n)
{
    const size_t CAP = (size_t)1 << 19;
    uint64_t M, x, B;
    int top, bits = 0;
    a135_u128 N, tot;
    a135_u128 *w, *A, *Bf, *tmp;
    size_t na;
    uint64_t hi, lo;
    seq_t r;

    if (n < 1) return 0;
    M = a135_isqrt((uint64_t)n * n * n);
    { uint64_t t = n; while (t) { bits++; t >>= 1; } }
    top = 64 + bits + 1;
    if (top > 126) top = 126;
    N = (a135_u128)n << 64;

    w = (a135_u128 *)malloc((size_t)(M + 2) * sizeof(a135_u128));
    for (x = 1; x <= M; x++) {
        uint64_t c = 1;
        while ((c + 1) * (c + 1) * (c + 1) <= x) c++;
        if (c * c * c == x) w[x] = (a135_u128)(c * c) << 64;
        else w[x] = a135_weight(x, top);
    }

    A = (a135_u128 *)malloc(2 * CAP * sizeof(a135_u128));
    Bf = (a135_u128 *)malloc(2 * CAP * sizeof(a135_u128));
    A[0] = 0;
    na = 1;
    B = 0;
    for (x = 1; x <= M; x++) {
        size_t i = 0, k = 0, nb = 0;
        a135_u128 wx = w[x];
        while (i < na || (k < na && A[k] + wx <= N)) {
            int takeB = (k < na && A[k] + wx <= N);
            if (i < na && (!takeB || A[i] <= A[k] + wx)) {
                Bf[nb++] = A[i++];
            } else {
                Bf[nb++] = A[k++] + wx;
            }
            if (nb > CAP) break;
        }
        if (nb > CAP) break;
        tmp = A; A = Bf; Bf = tmp;
        na = nb;
        B = x;
    }

    a135_w = w;
    a135_M = M;
    a135_s = A;
    a135_ns = na;
    if (B >= M) tot = na;
    else tot = a135_dfs(B + 1, N);
    tot -= 1; /* exclude the empty set */

    free(w);
    free(A);
    free(Bf);

    hi = (uint64_t)(tot >> 64);
    lo = (uint64_t)tot;
    r = (seq_t)lo;
    if (hi) {
        seq_t h = (seq_t)hi;
        h <<= 32;
        h <<= 32;
        r += h;
    }
    return r;
}
