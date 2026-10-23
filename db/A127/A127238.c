/*
 * A127238  Diagonal sums of Thue-Morse binomial triangle A127236.
 * https://oeis.org/A127238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} A010060(binomial(n-k,k))
 * Range:     u8 n <= 959, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a small big‑integer type (base 2^32) to
   compute the binomial coefficients exactly and then counts the
   parity of their popcount.  It works with any seq_t (unsigned int,
   uint32_t, uint64_t, or an arbitrary precision class).  */

#include <stdint.h>
#include <stddef.h>

#define BI_MAX_WORDS 256   /* enough for n <= 10000 (≈2000 bits) */

typedef struct {
    uint32_t w[BI_MAX_WORDS];
    size_t len;            /* number of used words (little endian) */
} bi_t;

/* set bi to a small uint64_t value */
static void bi_set_uint(bi_t *b, uint64_t v) {
    b->len = 0;
    while (v && b->len < BI_MAX_WORDS) {
        b->w[b->len++] = (uint32_t)v;
        v >>= 32;
    }
    if (b->len == 0) b->len = 1;
    while (b->len > 1 && b->w[b->len-1] == 0) b->len--;
}

/* multiply bi by a small uint64_t value */
static void bi_mul_uint(bi_t *b, uint64_t m) {
    unsigned __int128 carry = 0;
    size_t i;
    for (i = 0; i < b->len; ++i) {
        unsigned __int128 prod = (unsigned __int128)b->w[i] * m + carry;
        b->w[i] = (uint32_t)prod;
        carry  = prod >> 32;
    }
    while (carry && i < BI_MAX_WORDS) {
        b->w[i++] = (uint32_t)carry;
        carry >>= 32;
    }
    if (i > b->len) b->len = i;
    while (b->len > 1 && b->w[b->len-1] == 0) b->len--;
}

/* divide bi by a small uint64_t divisor (exact division) */
static void bi_div_uint(bi_t *b, uint64_t d) {
    unsigned __int128 rem = 0;
    size_t i = b->len;
    while (i--) {
        unsigned __int128 cur = (rem << 32) | b->w[i];
        b->w[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    while (b->len > 1 && b->w[b->len-1] == 0) b->len--;
}

/* parity of popcount of bi (0 or 1) */
static uint8_t bi_popcount_parity(const bi_t *b) {
    uint8_t parity = 0;
    for (size_t i = 0; i < b->len; ++i) {
        parity ^= (__builtin_popcount(b->w[i]) & 1);
    }
    return parity;
}

seq_t a(idx_t n) {
    idx_t maxk = n / 2U;
    seq_t total = 0;

    bi_t cur;
    bi_set_uint(&cur, 1);

    for (idx_t k = 0; k <= maxk; ++k) {
        if (k > 0U) {
            uint64_t num1 = (uint64_t)(n - 2U * k + 2U);
            uint64_t num2 = (uint64_t)(n - 2U * k + 1U);
            uint64_t den1 = (uint64_t)k;
            uint64_t den2 = (uint64_t)(n - k + 1U);

            bi_mul_uint(&cur, num1);
            bi_mul_uint(&cur, num2);
            bi_div_uint(&cur, den1);
            bi_div_uint(&cur, den2);
        }

        if (bi_popcount_parity(&cur))
            total += (seq_t)1U;
    }
    return total;
}
