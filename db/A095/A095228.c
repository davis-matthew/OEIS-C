/*
 * A095228  n-th decimal digit of 1/n!.
 * https://oeis.org/A095228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n-th decimal digit of 1/n!
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define BIGINT_MAX_D 4
#define BASE 1000000000U  /* 1e9 */

typedef struct {
    uint32_t d[BIGINT_MAX_D];
    size_t len;
} BigInt;

/* Initialize BigInt from unsigned 64-bit integer */
static void bigint_init(BigInt *a, uint64_t v) {
    a->len = 0;
    if (v == 0) {
        a->len = 1;
        a->d[0] = 0;
        return;
    }
    while (v > 0) {
        a->d[a->len++] = (uint32_t)(v % BASE);
        v /= BASE;
    }
}

/* Multiply BigInt by a small integer (m <= 10000) */
static void bigint_mul_small(BigInt *a, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    while (carry > 0) {
        a->d[a->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* Compare two BigInts: return -1 if a<b, 0 if a==b, 1 if a>b */
static int bigint_compare(const BigInt *a, const BigInt *b) {
    if (a->len != b->len)
        return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] != b->d[i])
            return (a->d[i] > b->d[i]) ? 1 : -1;
    }
    return 0;
}

/* Subtract BigInt b from a (a = a - b). Assumes a >= b. */
static void bigint_sub_self(BigInt *a, const BigInt *b) {
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t temp = (int64_t)a->d[i] - borrow - (i < b->len ? b->d[i] : 0);
        if (temp < 0) {
            temp += BASE;
            borrow = 1;
        } else
            borrow = 0;
        a->d[i] = (uint32_t)temp;
    }
    while (a->len > 1 && a->d[a->len - 1] == 0)
        --a->len;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* For n >= 25 the value is guaranteed to be 0 */
    if (n >= 25)
        return (seq_t)0;

    if (n == 0)
        return (seq_t)0;  /* a(0) = 0 */

    /* Compute factorial fact = n! as BigInt */
    BigInt fact;
    bigint_init(&fact, 1);
    for (idx_t i = 2; i <= n; ++i)
        bigint_mul_small(&fact, (uint32_t)i);

    /* Remainder before starting decimal digits: 1 % fact */
    BigInt rem;
    if (n >= 2)          /* fact > 1 */
        bigint_init(&rem, 1);
    else                 /* fact == 1 when n == 1 */
        bigint_init(&rem, 0);

    uint8_t digit = 0;

    for (idx_t i = 0; i < n; ++i) {
        /* Multiply remainder by 10 */
        bigint_mul_small(&rem, 10);

        /* Compute the next decimal digit */
        if (bigint_compare(&rem, &fact) < 0) {
            digit = 0;
        } else {
            uint8_t q = 0;
            while (bigint_compare(&rem, &fact) >= 0) {
                bigint_sub_self(&rem, &fact);
                ++q;
            }
            digit = q;
        }
    }
    return (seq_t)digit;
}
