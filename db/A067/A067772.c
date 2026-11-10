/*
 * A067772  One half of the number of non-self-conjugate balanced partitions.
 * https://oeis.org/A067772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (A047993(n) - A000700(n))/2, where A047993(n) =
 *            Sum_{k>=1} (-1)^(k-1) * ( p(n-(3k^2-k)/2) - p(n-(3k^2+k)/2) ) (p
 *            = partition numbers, p(0)=1, p(i)=0 for i<0) and A000700(n) is
 *            the coefficient of x^n in Product_{k>=1} (1 + x^(2k-1)). Every
 *            value v is held as a pair (h, b) with v = 2h + b, b...
 * Range:     u8 n <= 32, u16 n <= 68, u32 n <= 172, u64 n <= 501,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 898 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Pair arithmetic: value = 2*h + b */
static void A067772_add(seq_t *h, unsigned char *b, seq_t h2, unsigned char b2)
{
    seq_t carry = (seq_t)((*b & b2) ? 1 : 0);
    *h = *h + h2 + carry;
    *b = (unsigned char)(*b ^ b2);
}

static void A067772_sub(seq_t *h, unsigned char *b, seq_t h2, unsigned char b2)
{
    seq_t borrow = (seq_t)((*b < b2) ? 1 : 0);
    *h = *h - h2 - borrow;
    *b = (unsigned char)(*b ^ b2);
}

/* Cached partition numbers p[0..p_max] in pair form */
static seq_t         *A067772_ph = NULL;
static unsigned char *A067772_pb = NULL;
static idx_t          A067772_pmax = 0;
static int            A067772_init = 0;

static void A067772_ensure(idx_t N)
{
    if (A067772_init && A067772_pmax >= N)
        return;
    seq_t *nh = mem_resize(seq_t, A067772_ph, (N + 1));
    if (!nh)
        exit(EXIT_FAILURE);
    A067772_ph = nh;
    unsigned char *nb = (unsigned char *)realloc(A067772_pb, (size_t)(N + 1));
    if (!nb)
        exit(EXIT_FAILURE);
    A067772_pb = nb;

    idx_t start;
    if (!A067772_init) {
        A067772_ph[0] = (seq_t)0;
        A067772_pb[0] = 1;            /* p(0) = 1 = 2*0 + 1 */
        A067772_init = 1;
        start = 1;
    } else {
        start = A067772_pmax + 1;
    }
    for (idx_t m = start; m <= N; ++m) {
        seq_t ph = 0, nh2 = 0;
        unsigned char pb = 0, nb2 = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > m)
                break;
            if (k & 1)
                A067772_add(&ph, &pb, A067772_ph[m - g1], A067772_pb[m - g1]);
            else
                A067772_add(&nh2, &nb2, A067772_ph[m - g1], A067772_pb[m - g1]);
            if (g2 <= m) {
                if (k & 1)
                    A067772_add(&ph, &pb, A067772_ph[m - g2], A067772_pb[m - g2]);
                else
                    A067772_add(&nh2, &nb2, A067772_ph[m - g2], A067772_pb[m - g2]);
            }
        }
        A067772_sub(&ph, &pb, nh2, nb2);
        A067772_ph[m] = ph;
        A067772_pb[m] = pb;
    }
    A067772_pmax = N;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    A067772_ensure(n);

    /* X = A047993(n) in pair form */
    seq_t xh = 0, xnh = 0;
    unsigned char xb = 0, xnb = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t g1 = k * (3 * k - 1) / 2;
        idx_t g2 = k * (3 * k + 1) / 2;
        if (g1 > n)
            break;
        if (k & 1)
            A067772_add(&xh, &xb, A067772_ph[n - g1], A067772_pb[n - g1]);
        else
            A067772_add(&xnh, &xnb, A067772_ph[n - g1], A067772_pb[n - g1]);
        if (g2 <= n) {
            if (k & 1)
                A067772_add(&xnh, &xnb, A067772_ph[n - g2], A067772_pb[n - g2]);
            else
                A067772_add(&xh, &xb, A067772_ph[n - g2], A067772_pb[n - g2]);
        }
    }
    A067772_sub(&xh, &xb, xnh, xnb);

    /* Y = A000700(n) in pair form */
    seq_t *ch = mem_new(seq_t, (n + 1));
    unsigned char *cb = (unsigned char *)calloc((size_t)(n + 1), 1);
    if (!cb)
        exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= n; ++i)
        ch[i] = (seq_t)0;
    cb[0] = 1;
    for (idx_t d = 1; d <= n; d += 2) {
        for (idx_t i = n; i >= d; i--) {
            A067772_add(&ch[i], &cb[i], ch[i - d], cb[i - d]);
        }
    }
    seq_t yh = ch[n];
    unsigned char yb = cb[n];
    mem_free(ch);
    free(cb);

    /* (X - Y)/2: X - Y is even, so the low bits cancel */
    A067772_sub(&xh, &xb, yh, yb);
    return xh;
}
