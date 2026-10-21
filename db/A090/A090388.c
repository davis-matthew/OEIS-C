/*
 * A090388  Decimal expansion of 1 + sqrt(3).
 * https://oeis.org/A090388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithmic extraction of decimal digits of 1 + sqrt(3)
   using the digit‑by‑digit square‑root extraction algorithm.  */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Big integer type – works with a fixed maximum size. */
#define BASE 1000000000U     /* 10^9, fits in 32‑bit unsigned */
#define MAX_DEC_DIGITS 20050 /* enough for 10 000 digits after the dot */
#define MAX_WORDS ((MAX_DEC_DIGITS+8)/9+5)  /* words needed for MAX_DEC_DIGITS */

typedef struct {
    size_t len;                 /* number of used words (0 means 0) */
    uint32_t d[MAX_WORDS];
} BigInt;

/* initialization with an unsigned value */
static void bigint_init(BigInt *a, uint32_t v) {
    if (v == 0) { a->len = 0; return; }
    a->len = 1;
    a->d[0] = v;
}

/* copy src into dest */
static void bigint_copy(BigInt *dest, const BigInt *src) {
    dest->len = src->len;
    memcpy(dest->d, src->d, src->len * sizeof(uint32_t));
}

/* multiply by a small integer (0 <= m <= 1000) */
static void bigint_mul_small(BigInt *a, uint32_t m) {
    if (m == 0 || a->len == 0) { a->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t cur = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(cur % BASE);
        carry = cur / BASE;
    }
    while (carry) {
        if (a->len >= MAX_WORDS) abort();    /* safety; should not happen */
        a->d[a->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* add a small integer (0 <= a < BASE) to big integer */
static void bigint_add_small(BigInt *a, uint32_t add) {
    if (add == 0) return;
    uint64_t carry = add;
    size_t i = 0;
    while (carry && i < a->len) {
        uint64_t cur = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(cur % BASE);
        carry = cur / BASE;
        ++i;
    }
    while (carry) {
        if (a->len >= MAX_WORDS) abort();
        a->d[a->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* compare two big integers: 1 if a>b, 0 if a==b, -1 if a<b */
static int bigint_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i]) return (a->d[i] > b->d[i]) ? 1 : -1;
    }
    return 0;
}

/* subtract b from a, assuming a >= b */
static void bigint_sub(BigInt *a, const BigInt *b) {
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t sub = (uint64_t)a->d[i] -
                       (i < b->len ? (uint64_t)b->d[i] : 0ULL) -
                       borrow;
        if ((int64_t)sub < 0) {
            sub += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->d[i] = (uint32_t)sub;
    }
    /* trim leading zeros */
    while (a->len && a->d[a->len-1] == 0) --a->len;
}

/* static cache for computed digits of sqrt(3) after the decimal point */
static struct {
    int initialized;
    BigInt root;          /* current root * 10^k   */
    BigInt rem;           /* current remainder     */
    BigInt base;          /* 20 * root             */
    BigInt tmp;           /* temporary working area*/
    char *digits;         /* array of decimal digits */
    size_t cap;           /* allocated capacity of digits[] */
    size_t len;           /* number of digits stored */
} cache = {0};

/* initialise the static cache */
static void cache_init(void) {
    if (cache.initialized) return;
    cache.initialized = 1;
    /* start from integer part of sqrt(3): 1.something */
    bigint_init(&cache.root, 1);      /* root = 1 */
    bigint_init(&cache.rem, 2);       /* remainder = 3-1^2 = 2 */
    cache.cap = 4096;
    cache.digits = (char*)malloc(cache.cap);
    cache.len = 0;
}

static void grow_digits(void) {
    if (cache.len < cache.cap) return;
    size_t new_cap = cache.cap * 2;
    char *new_arr = (char*)realloc(cache.digits, new_cap);
    if (!new_arr) abort();
    cache.digits = new_arr;
    cache.cap = new_cap;
}

/* compute next decimal digit of sqrt(3) and store it */
static void compute_next_digit(void) {
    /* bring down 00: rem *= 100 */
    bigint_mul_small(&cache.rem, 100);

    /* base = 20 * root */
    bigint_copy(&cache.base, &cache.root);
    bigint_mul_small(&cache.base, 20);

    int chosen = 0;
    /* test digits from 9 down to 0 */
    for (int x = 9; x >= 0; --x) {
        bigint_copy(&cache.tmp, &cache.base);   /* tmp = 20*root */
        bigint_add_small(&cache.tmp, x);        /* tmp += x  -> 20*root + x */
        bigint_mul_small(&cache.tmp, (uint32_t)x); /* tmp *= x -> (20*root + x)*x */

        if (bigint_cmp(&cache.tmp, &cache.rem) <= 0) {
            chosen = x;
            break;
        }
    }

    /* subtract chosen * (20*root + chosen) from remainder */
    bigint_sub(&cache.rem, &cache.tmp);

    /* update root: root = root*10 + chosen */
    bigint_mul_small(&cache.root, 10);
    bigint_add_small(&cache.root, (uint32_t)chosen);

    /* store the digit */
    grow_digits();
    cache.digits[cache.len++] = (char)chosen;
}

/* public sequence function */
seq_t a(idx_t n) {
    cache_init();
    if (n == 0) return 0;          /* under‑offset – undefined, return 0 */
    if (n == 1) return 2;          /* integer part of 1+sqrt(3) */

    /* ensure we have enough digits computed */
    size_t need = (size_t)(n - 2);  /* because a(2) is the first decimal digit */
    while (cache.len <= need) {
        compute_next_digit();
    }
    return (seq_t)(cache.digits[need]) + 0;  /* cast 0 to seq_t to satisfy type */
}
