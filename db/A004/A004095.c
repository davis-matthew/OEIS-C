/*
 * A004095  Sum of digits of Catalan numbers.
 * https://oeis.org/A004095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digitsum(C(n)), where C(0) = 1 and C(k+1) =
 *            C(k)*2*(2k+1)/(k+2) (Catalan numbers); C(n) is held as a
 *            base-10^9 bignum
 * Range:     u8 n <= 94, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *cat_limbs = NULL;   /* little-endian base 1e9 */
static size_t cat_len = 0;
static size_t cat_cap = 0;
static uint64_t cat_k = 0;           /* cat_limbs holds C(cat_k) */

static void cat_reset(void) {
    if (cat_cap < 16) {
        free(cat_limbs);
        cat_cap = 16;
        cat_limbs = (uint32_t *)malloc(cat_cap * sizeof(uint32_t));
    }
    cat_limbs[0] = 1;
    cat_len = 1;
    cat_k = 0;
}

static void cat_mul_small(uint64_t m) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < cat_len; i++) {
        uint64_t t = (uint64_t)cat_limbs[i] * m + carry;
        cat_limbs[i] = (uint32_t)(t % 1000000000ULL);
        carry = t / 1000000000ULL;
    }
    while (carry) {
        if (cat_len == cat_cap) {
            cat_cap *= 2;
            cat_limbs = (uint32_t *)realloc(cat_limbs, cat_cap * sizeof(uint32_t));
        }
        cat_limbs[cat_len++] = (uint32_t)(carry % 1000000000ULL);
        carry /= 1000000000ULL;
    }
}

static void cat_div_small(uint64_t d) {
    uint64_t rem = 0;
    size_t i = cat_len;
    while (i > 0) {
        i--;
        uint64_t cur = rem * 1000000000ULL + cat_limbs[i];
        cat_limbs[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    while (cat_len > 1 && cat_limbs[cat_len - 1] == 0)
        cat_len--;
}

seq_t a(idx_t n) {
    if (cat_limbs == NULL || n < cat_k)
        cat_reset();
    while (cat_k < n) {
        cat_mul_small(2 * (2 * cat_k + 1));
        cat_div_small(cat_k + 2);
        cat_k++;
    }
    seq_t r = 0;
    size_t i;
    for (i = 0; i < cat_len; i++) {
        uint32_t v = cat_limbs[i];
        while (v) {
            r += (int)(v % 10);
            v /= 10;
        }
    }
    return r;
}
