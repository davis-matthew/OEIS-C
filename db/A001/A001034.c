/*
 * A001034  Orders of noncyclic simple groups (without repetition).
 * https://oeis.org/A001034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, core
 * Formula:   search. List the orders of every finite non-abelian simple group
 *            up to a bound B: alternating groups n!/2, the Lie-type families
 *            from their order formulas (L_n(q), U_n(q), B_n(q)=C_n(q),
 *            D_n(q), 2D_n(q), G2, F4, E6, 2E6, E7, E8, 3D4, Sz, Ree, 2F4,
 *            plus the Tits group) and the 26 sporadic groups....
 * Range:     u8 n <= 2, u16 n <= 28, u32 n <= 393, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp64, g1034, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/u128_to_seq.h"
#include "../utils/cmp64.h"
#include "../utils/g1034.h"

typedef unsigned __int128 u128;

static u128 CAPV, BND;
static u128 *arr;
static size_t cnt, capa;
static uint64_t *pp;
static size_t npp;

static void push(u128 v)
{
    if (cnt == capa) {
        capa = capa ? capa * 2 : 1024;
        arr = (u128 *)realloc(arr, capa * sizeof(u128));
    }
    arr[cnt++] = v;
}

static u128 mulc(u128 a, u128 b)
{
    if (a > CAPV || b > CAPV) return CAPV + 1;
    if (b && a > CAPV / b) return CAPV + 1;
    return a * b;
}

static u128 powc(u128 q, unsigned e)
{
    u128 r = 1;
    while (e--) {
        r = mulc(r, q);
        if (r > CAPV) break;
    }
    return r;
}

/* q^i - 1 (s = 0) or q^i + 1 (s = 1), capped */
static u128 fac(u128 q, unsigned i, int s)
{
    u128 p = powc(q, i);
    if (p > CAPV) return CAPV + 1;
    return s ? p + 1 : p - 1;
}

static void addo(u128 num, uint64_t d)
{
    u128 o;
    if (num > CAPV) return;
    o = num / d;
    if (o <= BND) push(o);
}

static u128 classical(int t, int m, uint64_t q, uint64_t *d)
{
    u128 num;
    int i;
    uint64_t r;
    switch (t) {
    case 0: /* L_m(q) */
        num = powc(q, (unsigned)(m * (m - 1) / 2));
        for (i = 2; i <= m; i++) num = mulc(num, fac(q, i, 0));
        *d = g1034((uint64_t)m, q - 1);
        return num;
    case 1: /* U_m(q) */
        num = powc(q, (unsigned)(m * (m - 1) / 2));
        for (i = 2; i <= m; i++) num = mulc(num, fac(q, i, i & 1));
        *d = g1034((uint64_t)m, q + 1);
        return num;
    case 2: /* B_m(q), C_m(q) */
        num = powc(q, (unsigned)(m * m));
        for (i = 1; i <= m; i++) num = mulc(num, fac(q, 2 * i, 0));
        *d = (q & 1) ? 2 : 1;
        return num;
    default: /* D_m(q) (t=3), 2D_m(q) (t=4) */
        num = powc(q, (unsigned)(m * (m - 1)));
        num = mulc(num, fac(q, m, t == 4));
        for (i = 1; i < m; i++) num = mulc(num, fac(q, 2 * i, 0));
        r = 1;
        for (i = 0; i < m; i++) r = r * (q % 4) % 4;
        *d = g1034(4, t == 3 ? (r + 3) % 4 : (r + 1) % 4);
        return num;
    }
}

static u128 exceptional(int t, uint64_t q, uint64_t *d)
{
    static const int e6[] = {12, 9, 8, 6, 5, 2};
    static const int e7[] = {2, 6, 8, 10, 12, 14, 18};
    static const int e8[] = {2, 8, 12, 14, 18, 20, 24, 30};
    static const int f4[] = {12, 8, 6, 2};
    u128 num, x;
    int i;
    *d = 1;
    switch (t) {
    case 0: /* G2 */
        num = powc(q, 6);
        num = mulc(num, fac(q, 6, 0));
        return mulc(num, fac(q, 2, 0));
    case 1: /* F4 */
        num = powc(q, 24);
        for (i = 0; i < 4; i++) num = mulc(num, fac(q, f4[i], 0));
        return num;
    case 2: /* E6 */
        num = powc(q, 36);
        for (i = 0; i < 6; i++) num = mulc(num, fac(q, e6[i], 0));
        *d = g1034(3, q - 1);
        return num;
    case 3: /* 2E6 */
        num = powc(q, 36);
        for (i = 0; i < 6; i++)
            num = mulc(num, fac(q, e6[i], e6[i] == 9 || e6[i] == 5));
        *d = g1034(3, q + 1);
        return num;
    case 4: /* E7 */
        num = powc(q, 63);
        for (i = 0; i < 7; i++) num = mulc(num, fac(q, e7[i], 0));
        *d = g1034(2, q - 1);
        return num;
    case 5: /* E8 */
        num = powc(q, 120);
        for (i = 0; i < 8; i++) num = mulc(num, fac(q, e8[i], 0));
        return num;
    default: /* 3D4 */
        num = powc(q, 12);
        x = powc(q, 8);
        if (x > CAPV) return CAPV + 1;
        x = x + powc(q, 4) + 1;
        num = mulc(num, x);
        num = mulc(num, fac(q, 6, 0));
        return mulc(num, fac(q, 2, 0));
    }
}

static int cmp128(const void *a, const void *b)
{
    u128 x = *(const u128 *)a, y = *(const u128 *)b;
    return x < y ? -1 : (x > y ? 1 : 0);
}

static uint64_t icbrt(u128 v)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 40;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        u128 c = (u128)mid * mid * mid;
        if (c <= v) lo = mid; else hi = mid - 1;
    }
    return lo;
}

static void build_pp(uint64_t Q)
{
    char *s = (char *)calloc((size_t)Q + 1, 1);
    uint64_t i, j;
    size_t cp = 1024;
    free(pp);
    pp = (uint64_t *)malloc(cp * sizeof(uint64_t));
    npp = 0;
    for (i = 2; i <= Q; i++) {
        if (s[i]) continue;
        for (j = i * i; j <= Q; j += i) s[j] = 1;
        {
            uint64_t p = i;
            for (;;) {
                if (npp == cp) {
                    cp *= 2;
                    pp = (uint64_t *)realloc(pp, cp * sizeof(uint64_t));
                }
                pp[npp++] = p;
                if (p > Q / i) break;
                p *= i;
            }
        }
    }
    free(s);
    qsort(pp, npp, sizeof(uint64_t), cmp64);
}

static u128 dec2(uint64_t hi, uint64_t lo)
{
    return (u128)hi * (u128)1000000000000000000ULL + lo;
}

static void gen(void)
{
    static const uint64_t spor[] = {
        7920ULL, 95040ULL, 443520ULL, 10200960ULL, 244823040ULL, 175560ULL, 604800ULL,
        50232960ULL, 44352000ULL, 898128000ULL, 448345497600ULL, 145926144000ULL,
        4157776806543360000ULL, 42305421312000ULL, 495766656000ULL, 4030387200ULL,
        64561751654400ULL, 4089470473293004800ULL, 460815505920ULL,
        51765179004000000ULL, 90745943887872000ULL, 273030912000000ULL
    };
    static const int mmin[] = {2, 3, 2, 4, 4};
    u128 f, num;
    uint64_t k, d, q;
    size_t i, j;
    int t, m;

    cnt = 0;
    CAPV = BND * 256;
    build_pp(icbrt(BND * 2) + 2);

    /* alternating */
    f = 60;
    k = 5;
    while (f <= BND) {
        push(f);
        k++;
        f = mulc(f, k);
    }

    /* sporadic + Tits */
    for (i = 0; i < sizeof(spor) / sizeof(spor[0]); i++)
        if ((u128)spor[i] <= BND) push(spor[i]);
    {
        u128 big[3];
        big[0] = dec2(86ULL, 775571046077562880ULL);                /* J4 */
        big[1] = dec2(1255205ULL, 709190661721292800ULL);           /* Fi24' */
        big[2] = dec2(4154781481226426ULL, 191177580544000000ULL);  /* B */
        for (i = 0; i < 3; i++) if (big[i] <= BND) push(big[i]);
    }
    if ((u128)17971200ULL <= BND) push(17971200ULL);

    /* classical */
    for (t = 0; t < 5; t++) {
        for (m = mmin[t];; m++) {
            uint64_t dmax = (t <= 1) ? (uint64_t)m : (t == 2 ? 2 : 4);
            u128 lim = BND * dmax;
            num = classical(t, m, 2, &d);
            if (num > lim) break;
            for (j = 0; j < npp; j++) {
                q = pp[j];
                num = classical(t, m, q, &d);
                if (num > lim) break;
                if (t == 0 && m == 2 && q < 4) continue;
                if (t == 1 && m == 3 && q == 2) continue;
                if (t == 2 && m == 2 && q == 2) continue;
                addo(num, d);
            }
        }
    }

    /* exceptional */
    for (t = 0; t < 7; t++) {
        u128 lim = BND * 3;
        for (j = 0; j < npp; j++) {
            q = pp[j];
            num = exceptional(t, q, &d);
            if (num > lim) break;
            if (t == 0 && q == 2) continue;
            addo(num, d);
        }
    }

    /* Suzuki 2B2(q), q = 2^(2m+1) */
    for (f = 8;; f *= 4) {
        num = mulc(mulc(powc(f, 2), fac(f, 2, 1)), f - 1);
        if (num > BND) break;
        push(num);
    }
    /* Ree 2G2(q), q = 3^(2m+1) */
    for (f = 27;; f *= 9) {
        num = mulc(mulc(powc(f, 3), fac(f, 3, 1)), f - 1);
        if (num > BND) break;
        push(num);
    }
    /* Ree 2F4(q), q = 2^(2m+1) */
    for (f = 8;; f *= 4) {
        num = powc(f, 12);
        num = mulc(num, fac(f, 6, 1));
        num = mulc(num, fac(f, 4, 0));
        num = mulc(num, fac(f, 3, 1));
        num = mulc(num, f - 1);
        if (num > BND) break;
        push(num);
    }

    qsort(arr, cnt, sizeof(u128), cmp128);
    j = 0;
    for (i = 0; i < cnt; i++)
        if (j == 0 || arr[i] != arr[j - 1]) arr[j++] = arr[i];
    cnt = j;
}

seq_t a(idx_t n)
{
    if (n < 1) n = 1;
    while ((idx_t)cnt < n) {
        BND = BND ? BND * 8 : ((u128)1 << 20);
        gen();
    }
    return u128_to_seq(arr[n - 1]);
}
