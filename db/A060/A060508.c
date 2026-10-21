/*
 * A060508  Exponent of largest power of n < 2^n.
 * https://oeis.org/A060508
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( n / log₂(n) ) Equivalently: the largest integer k
 *            such that nᵏ ≤ 2ⁿ
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------------------------------------------------------------------- */
/* Very small arbitrary‑precision integer (base 2³²)                        */
/* ---------------------------------------------------------------------- */

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint32_t *d;   /* low‑order limb first, base 2³²   */
    size_t   len;  /* number of limbs (len==0 → 0)    */
} BigInt;

/* initialise an empty big integer */
static inline void big_init(BigInt *b) { b->d = NULL; b->len = 0; }

/* free the memory owned by a big integer */
static inline void big_free(BigInt *b) { free(b->d); b->len = 0; }

/* construct a big integer from a 32‑bit unsigned value */
static void big_from_uint(BigInt *b, uint32_t v)
{
    if (v == 0) { big_init(b); return; }
    b->d = (uint32_t*)malloc(sizeof(uint32_t));
    b->len = 1;
    b->d[0] = v;
}

/* multiply a big integer by a small 32‑bit integer, in place */
static void big_mul_small(BigInt *b, uint32_t m)
{
    if (m == 0) { big_free(b); return; }
    if (b->len == 0) { big_from_uint(b, 0); return; }

    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)prod;
        carry   = prod >> 32;
    }
    if (carry) {
        b->d = (uint32_t*)realloc(b->d, (b->len + 1) * sizeof(uint32_t));
        b->d[b->len] = (uint32_t)carry;
        b->len++;
    }
}

/* compare two big integers : a < b  → -1
                                 a = b  → 0
                                 a > b  → 1
*/
static int big_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i])
            return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* ---------------------------------------------------------------------- */
/* OEIS sequence function                                                */
/* ---------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* undefined below the offset */

    /* ----- 2ⁿ  ----- */
    BigInt pow2; big_from_uint(&pow2, 1);
    for (idx_t i = 0; i < n; ++i) big_mul_small(&pow2, 2);

    /* ----- nᵏ while ≤ 2ⁿ ----- */
    BigInt cur; big_from_uint(&cur, 1);
    unsigned long long k = 0;   /* use 64‑bit counter, fits easily */
    while (big_cmp(&cur, &pow2) <= 0) {
        k++;                        /* next exponent   */
        big_mul_small(&cur, (uint32_t)n);
    }

    big_free(&pow2);
    big_free(&cur);

    return (seq_t)(k - 1);
}
