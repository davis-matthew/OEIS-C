/*
 * A084185  First occurrence of binary n in the binary expansion of sqrt(2).
 * https://oeis.org/A084185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* First occurrence of binary n in binary expansion of sqrt(2) */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint64_t *d;   /* little‑endian 64‑bit words */
    size_t len;    /* number of used words */
    size_t cap;    /* allocated capacity */
} bignum;

static void ensure_bignum_capacity(bignum *b, size_t needed) {
    if (b->cap < needed) {
        size_t newcap = b->cap ? b->cap : 16;
        while (newcap < needed) newcap <<= 1;
        b->d = (uint64_t *)realloc(b->d, newcap * sizeof(uint64_t));
        b->cap = newcap;
    }
}

static void bignum_trim(bignum *b) {
    while (b->len > 1 && b->d[b->len - 1] == 0) b->len--;
}

static void bignum_shift_left(bignum *b, int bits) {
    if (bits == 0) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t newcarry = b->d[i] >> (64 - bits);
        b->d[i] = (b->d[i] << bits) | carry;
        carry = newcarry;
    }
    if (carry) {
        ensure_bignum_capacity(b, b->len + 1);
        b->d[b->len++] = carry;
    }
    bignum_trim(b);
}

static void bignum_add_small(bignum *b, uint32_t val) {
    uint64_t carry = val;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t tmp = b->d[i] + carry;
        if (tmp < carry) carry = 1;
        else carry = 0;
        b->d[i] = tmp;
        if (!carry) break;
    }
    if (carry) {
        ensure_bignum_capacity(b, b->len + 1);
        b->d[b->len++] = carry;
    }
}

static int bignum_cmp(const bignum *a, const bignum *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i > 0; --i) {
        uint64_t va = a->d[i-1];
        uint64_t vb = b->d[i-1];
        if (va != vb) return (va < vb) ? -1 : 1;
    }
    return 0;
}

static void bignum_sub(bignum *a, const bignum *b) {
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t sub = (i < b->len) ? b->d[i] : 0;
        uint64_t tmp = sub + borrow;
        if (a->d[i] < tmp) {
            a->d[i] = a->d[i] - tmp;
            borrow = 1;
        } else {
            a->d[i] -= tmp;
            borrow = 0;
        }
    }
    bignum_trim(a);
}

/* Global state for sqrt(2) digit generation */
static bignum root, rem, cand;
static char *sqrt_bits = NULL;
static size_t bits_len = 0, bits_cap = 0;
static int init_done = 0;

/* Initialize global buffers */
static void init_globals(void) {
    if (init_done) return;
    root.cap = 16; root.len = 1; root.d = (uint64_t *)calloc(root.cap, sizeof(uint64_t));
    rem.cap = 16; rem.len = 1; rem.d = (uint64_t *)calloc(rem.cap, sizeof(uint64_t));
    cand.cap = 16; cand.len = 0; cand.d = (uint64_t *)calloc(cand.cap, sizeof(uint64_t));
    bits_cap = 16; sqrt_bits = (char *)calloc(bits_cap, 1);
    bits_len = 0;
    init_done = 1;
}

/* Generate the next binary digit of sqrt(2) */
static void generate_next_digit(void) {
    if (root.len == 0) { root.len = 1; root.d[0] = 0; }

    bignum_shift_left(&rem, 2);
    if (bits_len == 0)
        bignum_add_small(&rem, 2);
    else
        bignum_add_small(&rem, 0);

    if (cand.cap < root.cap) {
        free(cand.d);
        cand.cap = root.cap;
        cand.d = (uint64_t *)calloc(cand.cap, sizeof(uint64_t));
    }
    cand.len = root.len;
    memcpy(cand.d, root.d, root.len * sizeof(uint64_t));
    bignum_shift_left(&cand, 2);
    cand.d[0] |= 1;

    if (bignum_cmp(&rem, &cand) >= 0) {
        bignum_sub(&rem, &cand);
        bignum_shift_left(&root, 1);
        root.d[0] |= 1;
        if (bits_len >= bits_cap) {
            bits_cap <<= 1;
            sqrt_bits = (char *)realloc(sqrt_bits, bits_cap);
        }
        sqrt_bits[bits_len++] = 1;
    } else {
        bignum_shift_left(&root, 1);
        if (bits_len >= bits_cap) {
            bits_cap <<= 1;
            sqrt_bits = (char *)realloc(sqrt_bits, bits_cap);
        }
        sqrt_bits[bits_len++] = 0;
    }
}

/* Ensure at least 'target' bits of sqrt(2) are known */
static void ensure_bits(size_t target) {
    init_globals();
    while (bits_len < target) generate_next_digit();
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* out of offset range */

    int len = 0;
    idx_t tmp = n;
    while (tmp) { len++; tmp >>= 1; }
    if (len == 0) len = 1;
    char pattern[64];
    for (int i = 0; i < len; ++i) {
        pattern[i] = (char)((n >> (len - 1 - i)) & 1);
    }

    size_t pos = 1;
    while (1) {
        ensure_bits(pos + len - 1);
        int k;
        for (k = 0; k < len; ++k) {
            if (sqrt_bits[pos - 1 + k] != pattern[k]) break;
        }
        if (k == len) return (seq_t)pos;
        ++pos;
    }
}
