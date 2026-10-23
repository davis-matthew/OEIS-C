/*
 * A103681  Numbers m such that in binary representation m! does not contain
 *          7!.
 * https://oeis.org/A103681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 237, u16 n <= 1041, u32 n <= 1041, u64 n <= 1041,
 *            u128 n <= 1041, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A103681
 * Numbers m such that the binary representation of m! does NOT contain
 * the 13‑bit pattern 1001110110000 (the binary of 5040 = 7!).
 *
 * Implementation:  iterate m = 0,1,2,…  computing m! incrementally in a
 * small arbitrary‑precision integer, test each factorial for the
 * forbidden pattern and collect the qualifying m's.  The function keeps a
 * static list of the first n terms, so repeated calls in any order are
 * served in O(1) after the initial build.
 *
 * The code is valid both under C11 and C++17.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Small arbitrary‑precision unsigned integer (little‑endian limbs)      */
/* --------------------------------------------------------------------- */
typedef struct {
    uint32_t *d;    /* limbs, d[0] = least significant 32 bits */
    size_t   len;   /* number of used limbs (>=1) */
    size_t   cap;   /* allocated capacity of d   */
} BigInt;

static void bigint_reserve(BigInt *b, size_t new_cap)
{
    if (new_cap > b->cap) {
        b->cap = new_cap;
        b->d = (uint32_t *)realloc(b->d, b->cap * sizeof(uint32_t));
    }
}

static void bigint_init(BigInt *b)
{
    b->cap = 8;
    b->d   = (uint32_t *)malloc(b->cap * sizeof(uint32_t));
    b->len = 1;
    b->d[0] = 1U;   /* 0! = 1 */
}

static void bigint_mul_small(BigInt *b, uint32_t m)
{
    if (m == 0) {           /* should never happen for factorials > 0! */
        b->len = 1;
        b->d[0] = 0;
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)prod;
        carry   = prod >> 32;
    }
    if (carry) {
        bigint_reserve(b, b->cap + 1);
        b->d[b->len++] = (uint32_t)carry;
    }
}

/* number of bits needed to represent the integer (bitlen(0) == 0) */
static int bigint_bitlen(const BigInt *b)
{
    if (b->len == 0) return 0;
    uint32_t high = b->d[b->len - 1];
    int bits_high = 32 - __builtin_clz((unsigned int)high); /* high != 0 */
    return (int)((b->len - 1) * 32 + bits_high);
}

/* returns true if the binary representation contains the 13-bit pattern 1001110110000 */
static bool contains_forbidden_pattern(const BigInt *b)
{
    const uint16_t PATTERN  = 5040;          /* 0b1001110110000 */
    const int      PATLEN   = 13;
    int len = bigint_bitlen(b);
    if (len < PATLEN) return false;

    uint16_t window = 0;
    for (int bitpos = len - 1; bitpos >= 0; --bitpos) {
        size_t limb  = (size_t)bitpos >> 5;
        int    offset = bitpos & 31;
        uint32_t limb_val = b->d[limb];
        uint32_t bit = (limb_val >> offset) & 1U;
        window = ((window << 1) | bit) & ((1u << PATLEN) - 1);
        if (window == PATTERN) return true;
    }
    return false;
}

/* --------------------------------------------------------------------- */
/* Public function required by the harness                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static state retained across calls */
    static BigInt fact;                 /* current factorial m!          */
    static bool fact_inited = false;    /* true when fact is initialised*/
    static idx_t current_m = 0;         /* current integer m             */
    static seq_t *terms = NULL;         /* collected sequence values     */
    static size_t terms_cap = 0;
    static size_t terms_size = 0;

    if (!fact_inited) {
        fact_inited = true;
        bigint_init(&fact);   /* 0! = 1  */
    }

    /* grow the terms cache until it contains at least n entries      */
    while (terms_size < n) {
        /* test the factorial of current_m (first *these* is 0! when m==0) */
        if (!contains_forbidden_pattern(&fact)) {
            /* ensure terms array has capacity for the new value */
            if (terms_size >= terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 32;
                terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                terms_cap = new_cap;
            }
            terms[terms_size++] = (seq_t)current_m;
        }

        /* advance to the next integer: compute (current_m+1)!   */
        ++current_m;
        bigint_mul_small(&fact, (uint32_t)current_m);
    }

    return terms[n - 1];  /* 1‑based indexing as per OEIS */
}
