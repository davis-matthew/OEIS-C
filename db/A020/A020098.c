/*
 * A020098  Ceiling of Gamma(n+7/11)/Gamma(7/11).
 * https://oeis.org/A020098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌈Γ(n+7/11)/Γ(7/11)⌉ = ⌈∏_{k=0}^{n-1} (k + 7/11)⌉ =
 *            ⌈(∏_{k=0}^{n-1} (11k+7)) / 11^n⌉
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    uint32_t *d;   /* little‑endian digits, base = 1e9 */
    size_t size;
    size_t cap;
} BigInt;

static const uint32_t BIG_BASE = 1000000000U;

/* initialise big integer with value 1 */
static void big_init(BigInt *b)
{
    b->size = 1;
    b->cap  = 8;
    b->d    = (uint32_t *)malloc(b->cap * sizeof(uint32_t));
    b->d[0] = 1;
}

/* free big integer */
static void big_free(BigInt *b)
{
    free(b->d);
}

/* ensure capacity */
static void big_ensure_cap(BigInt *b, size_t need)
{
    if (need <= b->cap) return;
    size_t new_cap = b->cap * 2;
    if (new_cap < need) new_cap = need;
    b->d = (uint32_t *)realloc(b->d, new_cap * sizeof(uint32_t));
    b->cap = new_cap;
}

/* multiply big integer by small uint32_t factor */
static void big_mul_small(BigInt *b, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < b->size; ++i) {
        uint64_t tmp = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)(tmp % BIG_BASE);
        carry   = tmp / BIG_BASE;
    }
    while (carry) {
        big_ensure_cap(b, b->size + 1);
        b->d[b->size++] = (uint32_t)(carry % BIG_BASE);
        carry /= BIG_BASE;
    }
}

/* divide big integer by small uint32_t divisor, return remainder */
static uint32_t big_div_small(BigInt *b, uint32_t divisor, uint32_t *remainder)
{
    uint64_t rem = 0;
    for (size_t idx = b->size; idx > 0; --idx) {
        size_t i = idx - 1;
        uint64_t cur = rem * BIG_BASE + b->d[i];
        uint32_t q = (uint32_t)(cur / divisor);
        rem = cur % divisor;
        b->d[i] = q;
    }
    /* remove leading zeros */
    while (b->size > 1 && b->d[b->size - 1] == 0)
        b->size--;
    if (remainder) *remainder = (uint32_t)rem;
    return (uint32_t)rem;
}

/* add small uint32_t value to big integer */
static void big_add_small(BigInt *b, uint32_t add)
{
    uint64_t carry = add;
    size_t i = 0;
    while (carry && i < b->size) {
        uint64_t tmp = (uint64_t)b->d[i] + carry;
        b->d[i] = (uint32_t)(tmp % BIG_BASE);
        carry = tmp / BIG_BASE;
        i++;
    }
    while (carry) {
        big_ensure_cap(b, b->size + 1);
        b->d[b->size++] = (uint32_t)(carry % BIG_BASE);
        carry /= BIG_BASE;
    }
}

/* convert big integer to seq_t (supports arbitrary precision seq_t) */
static seq_t big_to_seq(const BigInt *b)
{
    seq_t val = 0;
    for (size_t i = b->size; i > 0; --i)
        val = val * (seq_t)BIG_BASE + (seq_t)b->d[i - 1];
    return val;
}

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;

    BigInt num;
    big_init(&num);

    /* numerator: product of (11*k + 7) for k = 0 .. n-1 */
    for (idx_t k = 0; k < n; ++k) {
        uint32_t term = (uint32_t)(11 * k + 7);
        big_mul_small(&num, term);
    }

    /* divide by 11^n step by step, record if any remainder occurs */
    bool had_remainder = false;
    uint32_t rem = 0;
    for (idx_t i = 0; i < n; ++i) {
        big_div_small(&num, 11, &rem);
        if (rem != 0)
            had_remainder = true;
    }

    /* ceil: if remainder existed, add 1 */
    if (had_remainder)
        big_add_small(&num, 1);

    seq_t result = big_to_seq(&num);
    big_free(&num);
    return result;
}
