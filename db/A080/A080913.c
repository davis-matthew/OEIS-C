/*
 * A080913  Value of index in A080900 when a number first appears for the n-th
 *          time.
 * https://oeis.org/A080913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   simulate A080900 (a(1)=1; a(k)=a(k-1)-2 if k already occurs,
 *            else a(k-1)+5) and record the index k where some value first
 *            reaches its n-th occurrence
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define A080913_WBITS 20
#define A080913_W ((uint64_t)1 << A080913_WBITS)
#define A080913_M (A080913_W - 1)

/* membership bitset over absolute values */
static unsigned char *A080913_bits = NULL;
static size_t A080913_bytes = 0;
/* occurrence counts for values in a sliding window below the running max */
static unsigned char *A080913_cnt = NULL;

static uint64_t A080913_i = 0;      /* last processed index of A080900 */
static uint64_t A080913_prev = 0;   /* A080900(A080913_i) */
static uint64_t A080913_maxv = 0;   /* largest value seen so far */
static uint64_t A080913_res[256];
static size_t A080913_nres = 0;

static void A080913_bits_ensure(uint64_t v)
{
    size_t need = (size_t)(v >> 3) + 1;
    if (need <= A080913_bytes) return;
    size_t cap = A080913_bytes ? A080913_bytes : ((size_t)1 << 16);
    while (cap < need) cap <<= 1;
    unsigned char *p = (unsigned char *)realloc(A080913_bits, cap);
    if (!p) abort();
    memset(p + A080913_bytes, 0, cap - A080913_bytes);
    A080913_bits = p;
    A080913_bytes = cap;
}

static void A080913_step(void)
{
    uint64_t i = A080913_i + 1;
    uint64_t v;
    if (i == 1) {
        v = 1;
    } else {
        bool in = (i <= A080913_maxv) &&
                  ((A080913_bits[(size_t)(i >> 3)] >> (i & 7)) & 1);
        v = in ? A080913_prev - 2 : A080913_prev + 5;
    }
    if (v > A080913_maxv) {
        uint64_t u;
        for (u = A080913_maxv + 1; u <= v; ++u) A080913_cnt[(size_t)(u & A080913_M)] = 0;
        A080913_maxv = v;
        A080913_bits_ensure(v);
    } else if (A080913_maxv - v >= A080913_W) {
        abort(); /* dip deeper than the count window: cannot happen in practice */
    }
    A080913_bits[(size_t)(v >> 3)] |= (unsigned char)(1u << (v & 7));
    unsigned char *c = &A080913_cnt[(size_t)(v & A080913_M)];
    if (*c < 255) {
        (*c)++;
        if ((size_t)*c == A080913_nres + 1) A080913_res[A080913_nres++] = i;
    }
    A080913_prev = v;
    A080913_i = i;
}

seq_t a(idx_t n)
{
    if (n == 0 || n > 255) return 0;
    if (!A080913_cnt) {
        A080913_cnt = (unsigned char *)calloc((size_t)A080913_W, 1);
        if (!A080913_cnt) abort();
        A080913_bits_ensure(1024);
    }
    while (A080913_nres < (size_t)n) A080913_step();
    return (seq_t)A080913_res[n - 1];
}
