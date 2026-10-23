/*
 * A152493  Numbers k such that the decimal expansion of 2^k+5^k contains no
 *          0's (probably 58 is last term).
 * https://oeis.org/A152493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 2^k + 5^k contains no decimal digit 0.
   The function incrementally builds 2^k and 5^k as arbitrary‑precision
   decimal numbers (arrays of base‑10 digits), sums them and tests for
   the presence of the digit 0. */

/* ---------- Big integer helpers (decimal digits) ---------- */
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    unsigned char d[128];   /* least‑significant digit first (base 10) */
    size_t len;             /* number of used digits, len>=1 */
} BigInt;

static void big_set_small(BigInt *x, int v) {
    x->len = 1;
    x->d[0] = (unsigned char)v;
}

static void big_mul_small(BigInt *x, int m) {
    int carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        int prod = x->d[i] * m + carry;
        x->d[i] = (unsigned char)(prod % 10);
        carry = prod / 10;
    }
    while (carry) {
        x->d[x->len++] = (unsigned char)(carry % 10);
        carry /= 10;
    }
}

static void big_add(BigInt *res, const BigInt *a, const BigInt *b) {
    size_t maxlen = a->len > b->len ? a->len : b->len;
    int carry = 0;
    size_t i;
    for (i = 0; i < maxlen; ++i) {
        int sum = carry;
        if (i < a->len) sum += a->d[i];
        if (i < b->len) sum += b->d[i];
        res->d[i] = (unsigned char)(sum % 10);
        carry = sum / 10;
    }
    while (carry) {
        res->d[i++] = (unsigned char)(carry % 10);
        carry /= 10;
    }
    res->len = i;
}

static bool has_zero_digits(const BigInt *x) {
    for (size_t i = 0; i < x->len; ++i)
        if (x->d[i] == 0)
            return true;
    return false;
}

/* ---------- Memoisation for the sequence ---------- */
static seq_t *terms = NULL;     /* stored k values (cast to seq_t) */
static size_t terms_sz = 0;     /* number of terms stored */
static size_t terms_cap = 0;    /* allocation capacity */

static void push_term(idx_t k) {
    if (terms_sz == terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 8 : terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_sz++] = (seq_t)k;
}

/* --------- Cached powers of 2 and 5 (decimal) ---------- */
static bool init_done = false;
static BigInt pow2, pow5;
static idx_t next_k = 0;       /* next k to test */

/* Expand the cache until we have at least n+1 terms. */
static void grow_until(idx_t n) {
    if (!init_done) {
        init_done = true;
        big_set_small(&pow2, 1);   /* 2^0 */
        big_set_small(&pow5, 1);   /* 5^0 */
        next_k = 0;
    }
    while (terms_sz <= (size_t)n) {
        BigInt sum;
        big_add(&sum, &pow2, &pow5);
        if (!has_zero_digits(&sum))
            push_term(next_k);
        ++next_k;
        big_mul_small(&pow2, 2);   /* 2^(k+1) = 2^k * 2 */
        big_mul_small(&pow5, 5);   /* 5^(k+1) = 5^k * 5 */
    }
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n) {
    grow_until(n);
    return terms[(size_t)n];
}
