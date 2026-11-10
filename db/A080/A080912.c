/*
 * A080912  a(n) = first number that appears n times in A080900.
 * https://oeis.org/A080912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = first value (in order of appearance) to reach n
 *            occurrences in A080900, where A080900: b(1)=1; b(m)=b(m-1)-2 if
 *            m already occurs in b, else b(m-1)+5. Computed by direct
 *            simulation with a seen-bitset and a sliding count window
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define A080912_W (1u << 22)
#define A080912_M (A080912_W - 1u)

static uint64_t *A080912_bits = NULL;
static uint64_t A080912_cap = 0;
static unsigned char *A080912_cnt = NULL;
static uint64_t A080912_best[256];
static uint64_t A080912_i = 0;
static uint64_t A080912_prev = 0;
static uint64_t A080912_hi = 0;

static bool A080912_seen(uint64_t v)
{
    uint64_t w = v >> 6;
    if (w >= A080912_cap) return false;
    return (A080912_bits[w] >> (v & 63)) & 1u;
}

static void A080912_mark(uint64_t v)
{
    uint64_t w = v >> 6;
    if (w >= A080912_cap) {
        uint64_t nc = A080912_cap ? A080912_cap * 2 : 1024;
        while (nc <= w) nc *= 2;
        uint64_t *nb = (uint64_t *)calloc((size_t)nc, sizeof(uint64_t));
        if (!nb) abort();
        if (A080912_bits) {
            memcpy(nb, A080912_bits, (size_t)A080912_cap * sizeof(uint64_t));
            free(A080912_bits);
        }
        A080912_bits = nb;
        A080912_cap = nc;
    }
    A080912_bits[w] |= (uint64_t)1 << (v & 63);
}

static void A080912_visit(uint64_t v)
{
    A080912_mark(v);
    if (v > A080912_hi) {
        for (uint64_t x = A080912_hi + 1; x <= v; ++x) A080912_cnt[x & A080912_M] = 0;
        A080912_hi = v;
    }
    unsigned char *c = &A080912_cnt[v & A080912_M];
    if (*c < 255) {
        ++*c;
        if (A080912_best[*c] == 0) A080912_best[*c] = v;
    }
}

seq_t a(idx_t n)
{
    if (n == 0 || n > 255) return 0;
    if (A080912_i == 0) {
        A080912_cnt = (unsigned char *)calloc(A080912_W, 1);
        if (!A080912_cnt) abort();
        A080912_i = 1;
        A080912_prev = 1;
        A080912_visit(1);
    }
    while (A080912_best[n] == 0) {
        uint64_t i = ++A080912_i;
        uint64_t nx = A080912_seen(i) ? A080912_prev - 2 : A080912_prev + 5;
        A080912_prev = nx;
        A080912_visit(nx);
    }
    return (seq_t)A080912_best[n];
}
