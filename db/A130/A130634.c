/*
 * A130634  Additive persistence of double factorials.
 * https://oeis.org/A130634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = additive persistence of n!! (double factorial)
 * Range:     u8 n <= 80, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation multiplies all factors of n!! using a small
   BigInt (base 1e9).  It then computes the digit sum of that product
   and counts how many times the sum must be repeated until a single
   digit remains.  The code is fully portable C11/C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Big integer in base 1 000 000 000. */
#define BASE ((uint32_t)1000000000U)

typedef struct {
    uint32_t *d;   /* least‑significant limbs first */
    size_t len;    /* number of limbs actually used      */
    size_t cap;    /* allocated number of limbs         */
} BigInt;

/* Allocate a BigInt initialised to `v`. */
static BigInt *big_new_uint32(uint32_t v)
{
    BigInt *b = (BigInt *)malloc(sizeof(BigInt));
    if (!b) return NULL;
    b->cap = 1;
    b->len = 1;
    b->d   = (uint32_t *)malloc(sizeof(uint32_t));
    if (!b->d) { free(b); return NULL; }
    b->d[0] = v;
    return b;
}

/* Multiply the BigInt `b` by a small integer `m`.  `m` must fit in 32 bits. */
static void big_mul_small(BigInt *b, uint32_t m)
{
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)(prod % BASE);
        carry   = prod / BASE;
    }
    while (carry > 0) {
        if (b->len == b->cap) {         /* grow capacity */
            size_t new_cap = b->cap * 2;
            uint32_t *newd = (uint32_t *)realloc(b->d, new_cap * sizeof(uint32_t));
            if (!newd) return;          /* allocation failed – abort */
            b->d = newd;
            b->cap = new_cap;
        }
        b->d[b->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* Free a BigInt. */
static void big_free(BigInt *b)
{
    if (!b) return;
    free(b->d);
    free(b);
}

/* Sum the decimal digits of a BigInt. */
static seq_t big_digit_sum(const BigInt *b)
{
    seq_t sum = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint32_t v = b->d[i];
        while (v > 0) {
            sum += v % 10;
            v   /= 10;
        }
    }
    return sum;
}

/* Sum the decimal digits of a small integer (fits in seq_t). */
static seq_t digit_sum_small(seq_t v)
{
    seq_t sum = 0;
    while (v > 0) {
        sum += v % 10;
        v   /= 10;
    }
    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Compute n!! as a BigInt.  The loop handles even and odd n
       correctly without resulting in an unsigned underflow. */
    BigInt *bi = big_new_uint32(1);
    if (!bi) return 0;   /* fall back in case of allocation failure */

    /* Multiply by n, n-2, ..., down to at least 2 (if n is even) or 3
       (if n is odd).  The last factor 1 (for odd n) does nothing
       and is omitted. */
    for (idx_t k = n; k >= 2; k -= 2) {
        big_mul_small(bi, (uint32_t)k);
        if (k <= 2) break;  /* guard against underflow when k==1 */
    }

    /* Determine whether the product is already a single digit. */
    bool more_than_one_digit = (bi->len > 1) || (bi->d[0] > 9);
    seq_t persistence = more_than_one_digit ? 1 : 0;

    /* First digit sum (always performed if product > 9).  For single
       digit products the persistence remains 0. */
    seq_t current = big_digit_sum(bi);
    big_free(bi);

    while (current >= 10) {
        current = digit_sum_small(current);
        persistence++;
    }
    return persistence;
}
