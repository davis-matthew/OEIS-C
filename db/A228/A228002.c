/*
 * A228002  Alternate partial sums of binomial(2n,n)^2.
 * https://oeis.org/A228002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Alternate partial sums of binomial(2n,n)^2 Recurrence (for
 *            n>=2): n^2*a(n) = (3*n-2)*(5*n-2)*a(n-1) + 4*(2*n-1)^2*a(n-2)
 *            Initial values: a(0) = 1, a(1) = 3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* arbitrary‑precision helper (base 1e9) */
/* --------------------------------------------------------- */
#define BIGBASE 1000000000U
#define BIGMAX 2000          /* enough for n ≤ 10000 */

typedef struct {
    uint32_t d[BIGMAX];
    size_t len;
} BigInt;

static void bi_zero(BigInt *a) { a->len = 0; }

static void bi_from_uint64(BigInt *a, uint64_t v) {
    bi_zero(a);
    if (v == 0) return;
    while (v) {
        a->d[a->len++] = (uint32_t)(v % BIGBASE);
        v /= BIGBASE;
    }
}

static void bi_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    for (size_t i = 0; i < src->len; ++i) dst->d[i] = src->d[i];
}

static void bi_trim(BigInt *a) {
    while (a->len && a->d[a->len-1] == 0) a->len--;
}

static void bi_add(const BigInt *a, const BigInt *b, BigInt *res) {
    uint64_t carry = 0;
    size_t i = 0;
    while (i < a->len || i < b->len || carry) {
        uint64_t sum = carry;
        if (i < a->len) sum += a->d[i];
        if (i < b->len) sum += b->d[i];
        res->d[i] = (uint32_t)(sum % BIGBASE);
        carry = sum / BIGBASE;
        ++i;
    }
    res->len = i;
}

static void bi_mul_small(const BigInt *a, uint32_t m, BigInt *res) {
    if (m == 0) {
        bi_zero(res);
        return;
    }
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        res->d[i] = (uint32_t)(prod % BIGBASE);
        carry = prod / BIGBASE;
    }
    while (carry) {
        res->d[i++] = (uint32_t)(carry % BIGBASE);
        carry /= BIGBASE;
    }
    res->len = i;
}

static void bi_div_small(const BigInt *a, uint32_t d, BigInt *res) {
    uint64_t rem = 0;
    size_t i;
    for (i = a->len; i > 0; --i) {
        rem = rem * BIGBASE + a->d[i-1];
        res->d[i-1] = (uint32_t)(rem / d);
        rem %= d;
    }
    res->len = a->len;
    bi_trim(res);
}

/* --------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    BigInt a_nm2, a_nm1, a_n;
    BigInt tmp1, tmp2, sum;
    bi_from_uint64(&a_nm2, 1);
    bi_from_uint64(&a_nm1, 3);

    for (idx_t i = 2; i <= n; ++i) {
        uint32_t f1 = (uint32_t)(3*i-2) * (uint32_t)(5*i-2);
        uint32_t f2 = (uint32_t)4 * (uint32_t)(2*i-1) * (uint32_t)(2*i-1);

        bi_mul_small(&a_nm1, f1, &tmp1);
        bi_mul_small(&a_nm2, f2, &tmp2);
        bi_add(&tmp1, &tmp2, &sum);

        uint32_t denom = (uint32_t)(i * i);
        bi_div_small(&sum, denom, &a_n);

        bi_copy(&a_nm2, &a_nm1);
        bi_copy(&a_nm1, &a_n);
    }

    /* Convert BigInt a_n to seq_t */
    seq_t result = 0;
    for (size_t i = a_n.len; i > 0; --i) {
        result = result * BIGBASE + (seq_t)a_n.d[i-1];
    }
    return result;
}
