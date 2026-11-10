/*
 * A088634  Index of the first occurrence of n in A066888.
 * https://oeis.org/A088634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 1 with pi(T(k)) - pi(T(k-1)) = n, where T(k) =
 *            k*(k+1)/2, i.e. the first k whose interval (T(k-1), T(k)] holds
 *            exactly n primes. The code finds k by segmented-sieve search,
 *            starting a safe margin below the k at which the expected count
 *            k/log(T(k)) reaches n
 * Range:     u8 n <= 30, u16 n <= 3101, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9957 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define A088634_BASE_LIM (1u << 22)
#define A088634_UNK 0xFFFFFFFFu
#define A088634_SEGNUM ((uint64_t)1 << 23)   /* numbers per segment */
#define A088634_PPER 15015u                  /* 3*5*7*11*13 */
#define A088634_PBITS (A088634_PPER * 64u)

static uint32_t *A088634_bp = NULL;   /* odd primes >= 3 */
static size_t A088634_nbp = 0;
static uint64_t *A088634_pat = NULL;  /* presieve pattern for 3..13 */
static uint32_t *A088634_cnt = NULL;  /* cnt[k] = #primes in (T(k-1),T(k)] */
static size_t A088634_cap = 0;
static uint64_t *A088634_memo = NULL;
static size_t A088634_memocap = 0;
static uint64_t *A088634_seg = NULL;
static size_t A088634_segcap = 0;

static uint64_t A088634_T(uint64_t k) { return k * (k + 1) / 2; }

static void A088634_setup(void)
{
    unsigned char *s = (unsigned char *)calloc(A088634_BASE_LIM + 1, 1);
    size_t i, j, c = 0;
    for (i = 2; (uint64_t)i * i <= A088634_BASE_LIM; i++)
        if (!s[i])
            for (j = i * i; j <= A088634_BASE_LIM; j += i) s[j] = 1;
    for (i = 3; i <= A088634_BASE_LIM; i += 2) if (!s[i]) c++;
    A088634_bp = (uint32_t *)malloc(c * sizeof(uint32_t));
    A088634_nbp = 0;
    for (i = 3; i <= A088634_BASE_LIM; i += 2)
        if (!s[i]) A088634_bp[A088634_nbp++] = (uint32_t)i;
    free(s);

    /* bit j set iff 1+2j divisible by one of 3,5,7,11,13 */
    A088634_pat = (uint64_t *)calloc(A088634_PPER + 1, sizeof(uint64_t));
    {
        static const unsigned sp[5] = {3, 5, 7, 11, 13};
        int q;
        for (q = 0; q < 5; q++) {
            uint64_t p = sp[q], jj;
            for (jj = (p - 1) / 2; jj < A088634_PBITS; jj += p)
                A088634_pat[jj >> 6] |= (uint64_t)1 << (jj & 63);
        }
    }
    A088634_pat[A088634_PPER] = A088634_pat[0];
}

static void A088634_ensure_cnt(uint64_t k)
{
    if (k < A088634_cap) return;
    size_t nc = A088634_cap ? A088634_cap : 1024;
    while (nc <= k) nc *= 2;
    A088634_cnt = (uint32_t *)realloc(A088634_cnt, nc * sizeof(uint32_t));
    for (size_t i = A088634_cap; i < nc; i++) A088634_cnt[i] = A088634_UNK;
    A088634_cap = nc;
}

/* number of set bits in bit range [a, b) */
static uint64_t A088634_pc(const uint64_t *s, uint64_t a, uint64_t b)
{
    if (a >= b) return 0;
    uint64_t wa = a >> 6, wb = b >> 6, r = 0, w;
    if (wa == wb)
        return (uint64_t)__builtin_popcountll(s[wa] & ((((uint64_t)1 << (b - a)) - 1) << (a & 63)));
    r += (uint64_t)__builtin_popcountll(s[wa] >> (a & 63));
    for (w = wa + 1; w < wb; w++) r += (uint64_t)__builtin_popcountll(s[w]);
    if (b & 63) r += (uint64_t)__builtin_popcountll(s[wb] & (((uint64_t)1 << (b & 63)) - 1));
    return r;
}

/* number of odd integers in [o0, x] */
static uint64_t A088634_jb(uint64_t o0, uint64_t x)
{
    return x < o0 ? 0 : (x - o0) / 2 + 1;
}

/* sieve intervals k..kend (whole intervals) and record their counts */
static void A088634_sieve_from(uint64_t k)
{
    uint64_t lo = A088634_T(k - 1) + 1;
    uint64_t kend = k, hi, o0, m, words, i, pos;
    while (A088634_T(kend + 1) - lo + 1 <= A088634_SEGNUM) kend++;
    hi = A088634_T(kend);
    o0 = lo | 1;
    m = hi >= o0 ? (hi - o0) / 2 + 1 : 0;
    words = (m + 63) / 64 + 1;
    if (words > A088634_segcap) {
        A088634_seg = (uint64_t *)realloc(A088634_seg, (size_t)words * sizeof(uint64_t));
        A088634_segcap = (size_t)words;
    }
    uint64_t *seg = A088634_seg;

    /* presieve 3,5,7,11,13 */
    pos = ((o0 - 1) / 2) % A088634_PPER;
    for (i = 0; i < words; i++) {
        uint64_t w = pos >> 6, b = pos & 63;
        uint64_t v = A088634_pat[w] >> b;
        if (b) v |= A088634_pat[w + 1] << (64 - b);
        seg[i] = v;
        pos += 64;
        if (pos >= A088634_PBITS) pos -= A088634_PBITS;
    }
    if (o0 <= 13) {
        static const unsigned sp[5] = {3, 5, 7, 11, 13};
        int q;
        for (q = 0; q < 5; q++)
            if (sp[q] >= o0 && sp[q] <= hi) {
                uint64_t jj = (sp[q] - o0) / 2;
                seg[jj >> 6] &= ~((uint64_t)1 << (jj & 63));
            }
    }
    if (o0 == 1 && m > 0) seg[0] |= 1;

    for (i = 0; i < A088634_nbp; i++) {
        uint64_t p = A088634_bp[i], j;
        if (p < 17) continue;
        if (p * p > hi) break;
        uint64_t m0 = ((o0 + p - 1) / p) * p;
        if (m0 < p * p) m0 = p * p;
        if (!(m0 & 1)) m0 += p;
        for (j = (m0 - o0) / 2; j < m; j += p)
            seg[j >> 6] |= (uint64_t)1 << (j & 63);
    }

    A088634_ensure_cnt(kend);
    {
        uint64_t kk;
        for (kk = k; kk <= kend; kk++) {
            uint64_t a0 = A088634_T(kk - 1), a1 = A088634_T(kk);
            uint64_t ja = A088634_jb(o0, a0), jbb = A088634_jb(o0, a1);
            uint64_t c = (jbb - ja) - A088634_pc(seg, ja, jbb);
            if (a0 < 2 && 2 <= a1) c++;
            A088634_cnt[kk] = (uint32_t)c;
        }
    }
}

static double A088634_mu(uint64_t k)
{
    double kd = (double)k;
    return kd / log(kd * (kd + 1.0) / 2.0);
}

static uint64_t A088634_klo(uint64_t n)
{
    double target = (double)n - (4.0 * sqrt((double)n) + 10.0);
    if (target <= 2.0) return 1;
    uint64_t L = 4, H = 8;
    while (A088634_mu(H) < target) H *= 2;
    while (L < H) {
        uint64_t mid = L + (H - L) / 2;
        if (A088634_mu(mid) >= target) H = mid; else L = mid + 1;
    }
    return L > 2 ? L - 1 : 1;
}

seq_t a(idx_t n)
{
    if (!A088634_bp) A088634_setup();
    if (n < A088634_memocap && A088634_memo[(size_t)n]) return (seq_t)A088634_memo[(size_t)n];
    uint64_t k = A088634_klo((uint64_t)n);
    for (;;) {
        A088634_ensure_cnt(k);
        if (A088634_cnt[k] == A088634_UNK) A088634_sieve_from(k);
        if ((uint64_t)A088634_cnt[k] == (uint64_t)n) break;
        k++;
    }
    if (n >= A088634_memocap) {
        size_t nc = A088634_memocap ? A088634_memocap : 256;
        while (nc <= n) nc *= 2;
        A088634_memo = (uint64_t *)realloc(A088634_memo, nc * sizeof(uint64_t));
        for (size_t i = A088634_memocap; i < nc; i++) A088634_memo[i] = 0;
        A088634_memocap = nc;
    }
    A088634_memo[(size_t)n] = k;
    return (seq_t)k;
}
