/*
 * A187765  The (n-1)th decimal place of the fractional part of the square
 *          root of n.
 * https://oeis.org/A187765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = The (n-1)th decimal digit of the fractional part of
 *            sqrt(n) (0 if n is a perfect square). Computed via
 *            digit‑by‑digit integer extraction using a small base‑10^9 bigint
 *            implementation
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- tiny bigint (base 1e9) implementation ---------- */
#define BIG_BASE 1000000000U     /* 1e9 fits in 32 bits */

typedef struct {
    uint32_t *d;      /* limbs, least‑significant first */
    size_t len;       /* used limbs */
    size_t cap;       /* allocated limbs */
} BigInt;

static void big_init(BigInt *a, size_t cap) {
    a->d = (uint32_t *)malloc(cap * sizeof(uint32_t));
    a->len = 0;
    a->cap = cap;
}

static void big_clear(BigInt *a) {
    free(a->d);
    a->d = NULL;
    a->len = a->cap = 0;
}

static void big_set_uint64(BigInt *a, uint64_t v) {
    size_t i = 0;
    a->len = 0;
    while (v) {
        if (i >= a->cap) {
            a->cap *= 2;
            a->d = (uint32_t *)realloc(a->d, a->cap * sizeof(uint32_t));
        }
        a->d[i++] = (uint32_t)(v % BIG_BASE);
        v /= BIG_BASE;
    }
    a->len = i;
    if (a->len == 0) a->len = 1, a->d[0] = 0;
    /* trim leading zeros */
    while (a->len > 1 && a->d[a->len - 1] == 0) a->len--;
}

static void big_copy(BigInt *dst, const BigInt *src) {
    if (dst->cap < src->len) {
        dst->cap = src->len;
        dst->d = (uint32_t *)realloc(dst->d, dst->cap * sizeof(uint32_t));
    }
    for (size_t i = 0; i < src->len; ++i) dst->d[i] = src->d[i];
    dst->len = src->len;
}

static void big_mul_small(BigInt *a, uint32_t m) {
    if (m == 0 || a->len == 1 && a->d[0] == 0) { a->len = 1; a->d[0] = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BIG_BASE);
        carry = prod / BIG_BASE;
    }
    if (carry) {
        if (a->len >= a->cap) {
            a->cap *= 2;
            a->d = (uint32_t *)realloc(a->d, a->cap * sizeof(uint32_t));
        }
        a->d[a->len++] = (uint32_t)carry;
    }
}

static void big_add_small(BigInt *a, uint32_t m) {
    uint64_t sum = (uint64_t)a->d[0] + m;
    a->d[0] = (uint32_t)(sum % BIG_BASE);
    uint64_t carry = sum / BIG_BASE;
    size_t i = 1;
    while (carry && i < a->len) {
        sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BIG_BASE);
        carry = sum / BIG_BASE;
        ++i;
    }
    if (carry) {
        if (a->len >= a->cap) {
            a->cap *= 2;
            a->d = (uint32_t *)realloc(a->d, a->cap * sizeof(uint32_t));
        }
        a->d[a->len++] = (uint32_t)carry;
    }
}

static int big_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

static void big_sub(BigInt *a, const BigInt *b) {
    /* assume *a >= *b */
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t diff = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (diff < 0) { diff += BIG_BASE; borrow = 1; } else borrow = 0;
        a->d[i] = (uint32_t)diff;
    }
    /* trim leading zeros */
    while (a->len > 1 && a->d[a->len - 1] == 0) a->len--;
}

/* ---------- integer sqrt for uint64_t ---------- */
static uint64_t uint64_isqrt(uint64_t x) {
    uint64_t lo = 0, hi = UINT64_MAX >> 1; /* sqrt(UINT64_MAX) is 4294967295 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > x / mid) hi = mid - 1;
        else lo = mid;
    }
    return lo;
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    /* special case n == 1 */
    if (n == 1) return (seq_t)0;

    /* integer square root and remainder */
    uint64_t N = (uint64_t)n;
    uint64_t root_ul = uint64_isqrt(N);
    uint64_t root_sq = root_ul * root_ul;
    if (root_sq == N) return (seq_t)0;            /* perfect square */

    uint64_t rem_ul = N - root_sq;

    /* big int buffers (depth sufficient for n <= 10000) */
    BigInt root, rem, c, tmp, prod;
    big_init(&root, 16);
    big_init(&rem, 16);
    big_init(&c, 16);
    big_init(&tmp, 16);
    big_init(&prod, 16);

    big_set_uint64(&root, root_ul);
    big_set_uint64(&rem, rem_ul);

    idx_t steps = n - 1;              /* number of decimal places to skip */
    uint32_t digit = 0;               /* result digit */

    for (idx_t k = 0; k < steps; ++k) {
        /* rem = rem * 100 */
        big_mul_small(&rem, 100);

        /* c = 20 * root */
        big_copy(&c, &root);
        big_mul_small(&c, 20);

        /* find largest d such that (c + d)*d <= rem */
        int found = 0;
        for (int d = 9; d >= 0; --d) {
            /* tmp = c + d */
            big_copy(&tmp, &c);
            big_add_small(&tmp, (uint32_t)d);

            /* prod = tmp * d */
            big_copy(&prod, &tmp);
            big_mul_small(&prod, (uint32_t)d);

            if (big_cmp(&prod, &rem) <= 0) {
                /* d is the digit */
                digit = (uint32_t)d;
                /* rem -= prod */
                big_sub(&rem, &prod);
                /* root = root * 10 + d */
                big_mul_small(&root, 10);
                big_add_small(&root, (uint32_t)d);
                found = 1;
                break;
            }
        }
        if (!found) digit = 0; /* should never happen */
    }

    /* clean up */
    big_clear(&root);
    big_clear(&rem);
    big_clear(&c);
    big_clear(&tmp);
    big_clear(&prod);

    return (seq_t)digit;
}
