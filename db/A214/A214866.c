/*
 * A214866  Numbers m such that A059175(m) = 0.
 * https://oeis.org/A214866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(1) = 0, then the numbers m >= 1 in increasing order
 *            for which iterating f(p/q) = p*q/(digitsum(p)+digitsum(q)) from
 *            m/1 never reaches an integer (a cycle is detected). The
 *            numerator is kept as an exact base-10^9 bignum
 * Range:     u8 n <= 9, u16 n <= 1759, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9294 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define A214866_MAXL 4096
#define A214866_BASE 1000000000u

typedef struct {
    int len;
    uint32_t d[A214866_MAXL];
} A214866_big;

static A214866_big A214866_cur, A214866_saved;

static uint64_t A214866_ds64(uint64_t x)
{
    uint64_t s = 0;
    while (x) { s += x % 10; x /= 10; }
    return s;
}

static uint64_t A214866_gcd64(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A214866_bigds(const A214866_big *b)
{
    uint64_t s = 0;
    for (int i = 0; i < b->len; ++i) s += A214866_ds64(b->d[i]);
    return s;
}

static uint64_t A214866_bigmod(const A214866_big *b, uint64_t m)
{
    uint64_t r = 0;
    for (int i = b->len - 1; i >= 0; --i)
        r = (r * A214866_BASE + b->d[i]) % m;
    return r;
}

/* returns false on overflow of the limb buffer */
static bool A214866_bigmul(A214866_big *b, uint64_t q)
{
    uint64_t carry = 0;
    for (int i = 0; i < b->len; ++i) {
        uint64_t t = (uint64_t)b->d[i] * q + carry;
        b->d[i] = (uint32_t)(t % A214866_BASE);
        carry = t / A214866_BASE;
    }
    while (carry) {
        if (b->len >= A214866_MAXL) return false;
        b->d[b->len++] = (uint32_t)(carry % A214866_BASE);
        carry /= A214866_BASE;
    }
    return true;
}

static void A214866_bigdiv(A214866_big *b, uint64_t g)
{
    uint64_t r = 0;
    for (int i = b->len - 1; i >= 0; --i) {
        uint64_t t = r * A214866_BASE + b->d[i];
        b->d[i] = (uint32_t)(t / g);
        r = t % g;
    }
    while (b->len > 1 && b->d[b->len - 1] == 0) b->len--;
}

static bool A214866_bigeq(const A214866_big *x, const A214866_big *y)
{
    if (x->len != y->len) return false;
    for (int i = 0; i < x->len; ++i)
        if (x->d[i] != y->d[i]) return false;
    return true;
}

static void A214866_bigcopy(A214866_big *dst, const A214866_big *src)
{
    dst->len = src->len;
    for (int i = 0; i < src->len; ++i) dst->d[i] = src->d[i];
}

/* true if the iteration from m/1 never reaches an integer (A059175(m) = 0) */
static bool A214866_never_integer(idx_t m)
{
    A214866_big *p = &A214866_cur, *sp = &A214866_saved;
    uint64_t mm = (uint64_t)m;
    p->len = 0;
    do {
        p->d[p->len++] = (uint32_t)(mm % A214866_BASE);
        mm /= A214866_BASE;
    } while (mm);
    uint64_t q = 1;
    A214866_bigcopy(sp, p);
    uint64_t sq = q;
    long long power = 1, lam = 0;

    for (int step = 0; step < 100000; ++step) {
        uint64_t s = A214866_bigds(p) + A214866_ds64(q);
        uint64_t pm = (A214866_bigmod(p, s) * (q % s)) % s;
        uint64_t g = (pm == 0) ? s : A214866_gcd64(pm, s);
        uint64_t nq = s / g;
        if (!A214866_bigmul(p, q)) return true;
        A214866_bigdiv(p, g);
        if (nq == 1) return false;
        q = nq;
        if (q == sq && A214866_bigeq(p, sp)) return true;
        ++lam;
        if (lam == power) {
            A214866_bigcopy(sp, p);
            sq = q;
            power *= 2;
            lam = 0;
        }
    }
    return true;
}

static idx_t *A214866_terms = NULL;
static size_t A214866_cnt = 0, A214866_cap = 0;
static idx_t A214866_next = 1;

seq_t a(idx_t n)
{
    if (A214866_cnt == 0) {
        A214866_cap = 256;
        A214866_terms = (idx_t *)malloc(A214866_cap * sizeof(idx_t));
        A214866_terms[A214866_cnt++] = 0;
    }
    while (A214866_cnt < (size_t)n) {
        idx_t m = A214866_next++;
        if (A214866_never_integer(m)) {
            if (A214866_cnt == A214866_cap) {
                A214866_cap *= 2;
                A214866_terms = (idx_t *)realloc(A214866_terms, A214866_cap * sizeof(idx_t));
            }
            A214866_terms[A214866_cnt++] = m;
        }
    }
    return (seq_t)A214866_terms[n - 1];
}
