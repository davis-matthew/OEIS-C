/*
 * A030324  Triangle read by rows, where row k consists of the binary digits
 *          of Fibonacci(k+1).
 * https://oeis.org/A030324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) = the (n−S)th bit of Fibonacci(k+1) in binary, where k is
 *            the unique integer with S = Σ_{i=1}^{k−1} ⌊log₂(F(i+1))⌋+1 < n ≤
 *            Σ_{i=1}^{k} ⌊log₂(F(i+1))⌋+1. Bits are taken from most
 *            significant to least significant (left‑to‑right in the binary
 *            representation)
 * Range:     u8 n <= 9853, u16 n <= 9853, u32 n <= 9853, u64 n <= 9853,
 *            u128 n <= 9853, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Big unsigned integer representation – enough for Fibonacci(200) (~118 bits). */
#define BIG_MAX_WORDS 256   /* 256 * 32 = 8192 bits – plenty for this sequence */

typedef struct {
    uint32_t w[BIG_MAX_WORDS]; /* little‑endian: w[0] contains bits 0..31 */
    size_t  len;               /* number of words actually used (len=0 for 0) */
} Big;

/* Initialise to 0 */
static void big_zero(Big *b) { b->len = 0; }

/* Initialise to an unsigned 32‑bit value */
static void big_set_uint32(Big *b, uint32_t v)
{
    if (v == 0) {
        b->len = 0;
    } else {
        b->w[0] = v;
        b->len = 1;
    }
}

/* b = a + c   (a and c are inputs, b is output) */
static void big_add(const Big *a, const Big *c, Big *b)
{
    size_t i, maxlen = a->len > c->len ? a->len : c->len;
    uint64_t carry = 0;
    for (i = 0; i < maxlen; ++i) {
        uint64_t av = (i < a->len) ? a->w[i] : 0;
        uint64_t cv = (i < c->len) ? c->w[i] : 0;
        uint64_t sum = av + cv + carry;
        b->w[i] = (uint32_t)sum;
        carry = sum >> 32;
    }
    if (carry) {
        b->w[i] = (uint32_t)carry;
        ++i;
    }
    b->len = i;
}

/* Number of bits needed to represent b */
static size_t big_bitlen(const Big *b)
{
    if (b->len == 0) return 0;
    uint32_t msw = b->w[b->len - 1];
    /* __builtin_clz is defined for 0 in GCC/Clang, but msw is never 0 here. */
    size_t leading = __builtin_clz(msw);
    return (b->len - 1) * 32 + (32 - leading);
}

/* The main sequence function */
seq_t a(idx_t n)
{
    idx_t remaining = n;
    Big prev, cur, next;
    /* Fibonacci(1)=1, Fibonacci(2)=1 */
    big_set_uint32(&prev, 1);
    big_set_uint32(&cur, 1);

    for (;;)
    {
        size_t len = big_bitlen(&cur);
        if (remaining <= (idx_t)len)
        {
            /* position counted from most significant bit */
            size_t abs_pos = len - remaining;       /* 0‑based from LSB */
            size_t word_idx = abs_pos >> 5;         /* /32 */
            size_t bit_idx  = abs_pos & 31;         /* %32 */
            seq_t bit = (cur.w[word_idx] >> bit_idx) & 1;
            return bit;
        }
        remaining -= (idx_t)len;

        /* next Fibonacci number: next = prev + cur */
        big_add(&prev, &cur, &next);

        /* rotate: prev<-cur, cur<-next */
        prev = cur;
        cur  = next;
    }
}
