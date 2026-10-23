/*
 * A152303  a(n) = b(n) mod 10, where b(n) = b(n-1) + b(n-2) +
 *          floor(b(n-2)/10), b(0) = b(1) = 1.
 * https://oeis.org/A152303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n) mod 10, where b(0) = b(1) = 1 b(n) = b(n‑1) + b(n‑2)
 *            + floor(b(n‑2)/10) (n ≥ 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* big‑integer helpers ---------------------------------------------------- */
/* base 1E9 representation (fits easily in 32‑bit words).               */
#define BASE        1000000000U
#define NBIG        300            /* enough for ~2700 decimal digits       */

typedef struct {
    uint32_t d[NBIG];
    size_t   len;                  /* number of used words (least significant first) */
} Big;

static void copy_big(Big *dst, const Big *src)
{
    dst->len = src->len;
    memcpy(dst->d, src->d, dst->len * sizeof(uint32_t));
}

static void add_big(const Big *a, const Big *b, Big *res)
{
    size_t maxlen = (a->len > b->len) ? a->len : b->len;
    uint64_t carry = 0;
    size_t i;

    for (i = 0; i < maxlen || carry; ++i) {
        uint64_t av = (i < a->len) ? a->d[i] : 0;
        uint64_t bv = (i < b->len) ? b->d[i] : 0;
        uint64_t sum = av + bv + carry;
        res->d[i] = (uint32_t)(sum % BASE);
        carry   =  sum / BASE;
    }
    res->len = i;
}

static void div10(const Big *in, Big *out)
{
    out->len = in->len;
    uint64_t carry = 0;
    if (in->len == 0) { out->len = 0; return; }
    for (int i = (int)in->len - 1; i >= 0; --i) {
        uint64_t cur = in->d[i] + carry * BASE;
        out->d[i] = (uint32_t)(cur / 10);
        carry     = cur % 10;
    }
    /* strip leading zero words */
    while (out->len > 0 && out->d[out->len - 1] == 0) --out->len;
}

/* --------------------------------------------------------------------- */
/* cache to allow arbitrary ordering of calls --------------------------- */

static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;
static idx_t  cached_max = (idx_t)-1;          /* uninitialised sentinel */

/* ensures that a_cache can hold indices up to n and sets up
   the first two elements when called for the first time.           */
static void ensure_cache(idx_t n)
{
    if (cached_max == (idx_t)-1) {
        /* first call : set array for indices 0,1 */
        a_cache_len = 2;
        a_cache = (seq_t *)malloc(sizeof(seq_t) * a_cache_len);
        a_cache[0] = 1;
        a_cache[1] = 1;
        cached_max = 1;
    }

    if ((size_t)n + 1 > a_cache_len) {
        size_t new_len = (a_cache_len > 0) ? a_cache_len * 2 : 2;
        while (new_len <= (size_t)n + 1) new_len <<= 1;
        a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_len);
        a_cache_len = new_len;
    }
}

/* --------------------------------------------------------------------- */
/* main entry ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n <= cached_max) return a_cache[n];

    /* big integers for the recurrence */
    static Big b_prev2 = { .len = 0 };   /* b(n-2)   */
    static Big b_prev1 = { .len = 0 };   /* b(n-1)   */

    if (cached_max == 1) {
        b_prev2.len = 1;  b_prev2.d[0] = 1;
        b_prev1.len = 1;  b_prev1.d[0] = 1;
    }

    Big temp, z, next;
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        /* next = b_prev1 + b_prev2 + floor(b_prev2/10) */
        add_big(&b_prev1, &b_prev2, &temp);
        div10(&b_prev2, &z);
        add_big(&temp,   &z,       &next);

        /* store only the last decimal digit of b(i) */
        a_cache[i] = (seq_t)(next.d[0] % 10);

        /* shift for next iteration */
        copy_big(&b_prev2, &b_prev1);
        copy_big(&b_prev1, &next);
    }
    cached_max = n;
    return a_cache[n];
}
