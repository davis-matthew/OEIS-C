/*
 * A005485  Denominators of continued fraction convergents to cube root of 7.
 * https://oeis.org/A005485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(-1) = 0, a(-2) = 1, where c(n)
 *            are the partial quotients of 7^(1/3), computed exactly: P(x) =
 *            x^3 - 7, c = floor(root of P), then P(x) <- x^3 * P(c + 1/x)
 *            (big-integer cubic, single real root)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *A5485_C[4];
static uint32_t *A5485_v, *A5485_w;
static size_t A5485_L = 0;
static uint64_t *A5485_pq = NULL;
static idx_t A5485_npq = 0;
static seq_t *A5485_q = NULL;
static idx_t A5485_nq = 0;

#define A5485_H 8

static void a5485_muladd(uint32_t *d, const uint32_t *s, uint64_t m)
{
    unsigned __int128 carry = 0;
    size_t i;
    for (i = 0; i < A5485_L; i++) {
        unsigned __int128 p = (unsigned __int128)s[i] * m + d[i] + carry;
        d[i] = (uint32_t)p;
        carry = p >> 32;
    }
}

static void a5485_grow(size_t newL)
{
    int j;
    for (j = 0; j < 4; j++) {
        uint32_t fill = (A5485_C[j][A5485_L - 1] >> 31) ? 0xFFFFFFFFu : 0u;
        size_t i;
        A5485_C[j] = (uint32_t *)realloc(A5485_C[j], newL * sizeof(uint32_t));
        for (i = A5485_L; i < newL; i++) A5485_C[j][i] = fill;
    }
    A5485_v = (uint32_t *)realloc(A5485_v, newL * sizeof(uint32_t));
    A5485_w = (uint32_t *)realloc(A5485_w, newL * sizeof(uint32_t));
    A5485_L = newL;
}

static int a5485_needs_grow(const uint32_t *c)
{
    uint32_t fill = (c[A5485_L - 1] >> 31) ? 0xFFFFFFFFu : 0u;
    size_t i;
    for (i = A5485_L - A5485_H; i < A5485_L; i++)
        if (c[i] != fill) return 1;
    return 0;
}

static void a5485_init(void)
{
    size_t i;
    int j;
    A5485_L = 16;
    for (j = 0; j < 4; j++) {
        A5485_C[j] = (uint32_t *)malloc(A5485_L * sizeof(uint32_t));
        memset(A5485_C[j], 0, A5485_L * sizeof(uint32_t));
    }
    A5485_C[3][0] = 1;
    for (i = 0; i < A5485_L; i++) A5485_C[0][i] = 0xFFFFFFFFu;
    A5485_C[0][0] = (uint32_t)(-7);
    A5485_v = (uint32_t *)malloc(A5485_L * sizeof(uint32_t));
    A5485_w = (uint32_t *)malloc(A5485_L * sizeof(uint32_t));
}

/* true if t is below the real root of P */
static int a5485_below(uint64_t t)
{
    int j;
    uint32_t s3 = A5485_C[3][A5485_L - 1] >> 31;
    memcpy(A5485_v, A5485_C[3], A5485_L * sizeof(uint32_t));
    for (j = 2; j >= 0; j--) {
        uint32_t *tmp;
        memcpy(A5485_w, A5485_C[j], A5485_L * sizeof(uint32_t));
        a5485_muladd(A5485_w, A5485_v, t);
        tmp = A5485_v; A5485_v = A5485_w; A5485_w = tmp;
    }
    return (A5485_v[A5485_L - 1] >> 31) != s3;
}

static void a5485_next_quotient(void)
{
    uint64_t lo, hi, c;
    int j, i, k;
    uint32_t *t0, *t1, *t2, *t3;

    for (j = 0; j < 4; j++) {
        if (a5485_needs_grow(A5485_C[j])) {
            a5485_grow(A5485_L + A5485_L / 2 + 16);
            break;
        }
    }

    lo = 1;
    hi = 2;
    while (a5485_below(hi)) { lo = hi; hi *= 2; }
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (a5485_below(mid)) lo = mid; else hi = mid;
    }
    c = lo;

    /* Taylor shift: P(x) -> P(x + c) */
    for (i = 0; i < 3; i++)
        for (k = 2; k >= i; k--)
            a5485_muladd(A5485_C[k], A5485_C[k + 1], c);

    /* reverse: x^3 * P(c + 1/x) */
    t0 = A5485_C[0]; t1 = A5485_C[1]; t2 = A5485_C[2]; t3 = A5485_C[3];
    A5485_C[3] = t0; A5485_C[2] = t1; A5485_C[1] = t2; A5485_C[0] = t3;

    A5485_pq = (uint64_t *)realloc(A5485_pq, (A5485_npq + 1) * sizeof(uint64_t));
    A5485_pq[A5485_npq++] = c;
}

seq_t a(idx_t n)
{
    idx_t k;
    if (n < A5485_nq) return A5485_q[n];
    if (A5485_L == 0) a5485_init();
    while (A5485_npq <= n) a5485_next_quotient();

    A5485_q = (seq_t *)realloc(A5485_q, (size_t)(n + 1) * sizeof(seq_t));
    for (k = A5485_nq; k <= n; k++) {
        seq_t p1 = (k >= 1) ? A5485_q[k - 1] : (seq_t)0;
        seq_t p2 = (k >= 2) ? A5485_q[k - 2] : (k == 1 ? (seq_t)0 : (seq_t)1);
        if (k == 0) { p1 = 0; p2 = 1; }
        A5485_q[k] = (seq_t)A5485_pq[k] * p1 + p2;
    }
    A5485_nq = n + 1;
    return A5485_q[n];
}
