/*
 * A000195  a(n) = floor(log(n)).
 * https://oeis.org/A000195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(log(n)) = #{k >= 1 : floor(e^k) < n}; floor(e^k)
 *            computed exactly via 256-bit fixed-point arithmetic
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

#define A195_FL 8
#define A195_IL 3
#define A195_L (A195_FL + A195_IL)
#define A195_KMAX 44

static uint64_t a195_thr[A195_KMAX + 1];
static int a195_ready = 0;

static void a195_mul(uint32_t *r, const uint32_t *x, const uint32_t *y)
{
    uint32_t t[2 * A195_L];
    int i, j;
    memset(t, 0, sizeof(t));
    for (i = 0; i < A195_L; i++) {
        uint64_t carry = 0;
        for (j = 0; j < A195_L; j++) {
            uint64_t cur = (uint64_t)x[i] * y[j] + t[i + j] + carry;
            t[i + j] = (uint32_t)cur;
            carry = cur >> 32;
        }
        t[i + A195_L] = (uint32_t)carry;
    }
    for (i = 0; i < A195_L; i++)
        r[i] = t[i + A195_FL];
}

static void a195_add(uint32_t *r, const uint32_t *x)
{
    uint64_t carry = 0;
    int i;
    for (i = 0; i < A195_L; i++) {
        uint64_t cur = (uint64_t)r[i] + x[i] + carry;
        r[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static void a195_div_small(uint32_t *r, uint32_t d)
{
    uint64_t rem = 0;
    int i;
    for (i = A195_L - 1; i >= 0; i--) {
        uint64_t cur = (rem << 32) | r[i];
        r[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void a195_init(void)
{
    uint32_t e[A195_L], term[A195_L], p[A195_L];
    uint32_t j;
    int k;
    memset(e, 0, sizeof(e));
    memset(term, 0, sizeof(term));
    term[A195_FL] = 1;
    for (j = 1; j <= 80; j++) {
        a195_add(e, term);
        a195_div_small(term, j);
    }
    memset(p, 0, sizeof(p));
    p[A195_FL] = 1;
    for (k = 1; k <= A195_KMAX; k++) {
        a195_mul(p, p, e);
        a195_thr[k] = ((uint64_t)p[A195_FL + 1] << 32) | p[A195_FL];
    }
    a195_ready = 1;
}

seq_t a(idx_t n)
{
    int k, c = 0;
    if (!a195_ready)
        a195_init();
    for (k = 1; k <= A195_KMAX; k++) {
        if ((uint64_t)n > a195_thr[k])
            c++;
        else
            break;
    }
    return (seq_t)c;
}
