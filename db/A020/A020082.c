/*
 * A020082  a(n) = floor( Gamma(n + 4/5)/Gamma(4/5) ).
 * https://oeis.org/A020082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Gamma(n + 4/5)/Gamma(4/5) )
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils/seq_from_str.h"   /* seq_t seq_from_str(const char *s) */

/* Big integer base 1e9 (fits in 32 bits) */
#define BIGINT_BASE 1000000000u

typedef struct {
    uint32_t *d;   /* little‑endian limbs, each < BIGINT_BASE */
    size_t   len;  /* number of limbs used */
} BigInt;

/* initialise to 1 */
static void big_init(BigInt *b)
{
    b->d = (uint32_t *)malloc(sizeof(uint32_t));
    b->d[0] = 1u;
    b->len = 1;
}

/* free memory */
static void big_free(BigInt *b)
{
    free(b->d);
    b->d = NULL;
    b->len = 0;
}

/* multiply by small int (m <= 5*n+4 <= 50004 for n <= 10000) */
static void big_mul_small(BigInt *b, uint32_t m)
{
    uint64_t carry = 0ULL;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t tmp = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)(tmp % BIGINT_BASE);
        carry   = tmp / BIGINT_BASE;
    }
    while (carry) {
        b->d = (uint32_t *)realloc(b->d, (b->len + 1) * sizeof(uint32_t));
        b->d[b->len++] = (uint32_t)(carry % BIGINT_BASE);
        carry /= BIGINT_BASE;
    }
}

/* divide by small int (s <= 5) */
static void big_div_small(BigInt *b, uint32_t s)
{
    uint64_t rem = 0ULL;
    for (size_t i = b->len; i-- > 0; ) {
        uint64_t cur = (uint64_t)b->d[i] + rem * BIGINT_BASE;
        b->d[i] = (uint32_t)(cur / s);
        rem     = cur % s;
    }
    /* remove leading zeros */
    while (b->len > 1 && b->d[b->len - 1] == 0)
        --b->len;
}

/* convert BigInt to decimal string (null‑terminated).  Caller must free. */
static char *big_to_dec_string(const BigInt *b)
{
    /* estimate buffer size: at most 9 digits per limb + 1 null */
    size_t buf_size = b->len * 9 + 2;
    char *buf = (char *)malloc(buf_size);
    if (!buf) return NULL;

    /* write most significant limb without leading zeros */
    int pos = 0;
    pos += sprintf(buf + pos, "%u", b->d[b->len - 1]);

    /* write remaining limbs, padded to 9 digits */
    for (size_t i = b->len - 1; i-- > 0; ) {
        pos += sprintf(buf + pos, "%09u", b->d[i]);
    }
    buf[pos] = '\0';
    return buf;
}

/* --------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* handle base case */
    if (n == 0) {
        return (seq_t)1;
    }

    BigInt prod;
    big_init(&prod);

    /* product = Π (5k + 4), k = 0 .. n-1 */
    for (idx_t k = 0; k < n; ++k) {
        uint32_t factor = 5u * (uint32_t)k + 4u;
        big_mul_small(&prod, factor);
    }

    /* divide by 5^n */
    for (idx_t i = 0; i < n; ++i) {
        big_div_small(&prod, 5u);
    }

    /* convert to decimal string and then to seq_t */
    char *s = big_to_dec_string(&prod);
    seq_t result = seq_from_str(s);
    free(s);
    big_free(&prod);
    return result;
}
