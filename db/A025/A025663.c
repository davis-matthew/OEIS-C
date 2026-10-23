/*
 * A025663  Exponent of 6 (value of i) in n-th number of form 6^i*10^j.
 * https://oeis.org/A025663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generated via a multiplicative (Hamming‑like) algorithm.
 *
 * The sequence a(n) gives the exponent i of 6 in the
 * n‑th smallest number of the form 6^i * 10^j (i,j ≥ 0).
 * Numbers are generated in increasing order; each
 * new term is obtained by multiplying an earlier term by 6
 * or by 10.  The procedure is identical to the classic
 * generation of Hamming numbers with the generators 6 and 10.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* ---------- Tiny Big Integer implementation ---------- */

typedef struct {
    uint32_t *d;   /* little‑endian limbs (base 2^32) */
    size_t len;    /* number of limbs */
} BigInt;

/* Allocate a BigInt from a single limb value */
static BigInt *bigint_from_uint(uint32_t v)
{
    BigInt *r = (BigInt *)malloc(sizeof(BigInt));
    r->len = (v == 0) ? 0 : 1;
    if (r->len) {
        r->d = (uint32_t *)malloc(sizeof(uint32_t));
        r->d[0] = v;
    } else
        r->d = NULL;
    return r;
}

/* Copy a BigInt */
static BigInt *bigint_copy(const BigInt *a)
{
    if (!a || a->len == 0) return bigint_from_uint(0);
    BigInt *r = (BigInt *)malloc(sizeof(BigInt));
    r->len = a->len;
    r->d = (uint32_t *)malloc(r->len * sizeof(uint32_t));
    memcpy(r->d, a->d, r->len * sizeof(uint32_t));
    return r;
}

/* Multiply a BigInt by a small unsigned integer (<= 10). */
static BigInt *bigint_mul_small(const BigInt *a, uint32_t m)
{
    if (m == 0) return bigint_from_uint(0);
    if (!a || a->len == 0) return bigint_from_uint(0);
    BigInt *r = (BigInt *)malloc(sizeof(BigInt));
    r->len = a->len + 1;          /* worst case when carry */
    r->d = (uint32_t *)calloc(r->len, sizeof(uint32_t));
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        r->d[i] = (uint32_t)prod;
        carry   = prod >> 32;
    }
    if (carry) r->d[a->len] = (uint32_t)carry;
    /* trim leading zero limb */
    while (r->len > 0 && r->d[r->len - 1] == 0) r->len--;
    return r;
}

/* Compare two BigInts: return <0 if a<b, 0 if a==b, >0 if a>b. */
static int bigint_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* Free a BigInt */
static void bigint_free(BigInt *a)
{
    if (!a) return;
    free(a->d);
    free(a);
}

/* ---------- Generation data structures ---------- */

typedef struct {
    int i;          /* exponent of 6     */
    int j;          /* exponent of 10    */
    BigInt *val;    /* full value 6^i*10^j */
} Item;

/* Sequence metadata */
static Item        *items = NULL;
static size_t      items_cap = 0;
static size_t      items_len = 0;

/* Indices for next candidates */
static size_t idx6  = 0;
static size_t idx10 = 0;

/* Exponent results for a(n) */
static seq_t *exponents = NULL;
static idx_t  exp_cap   = 0;
static idx_t  exp_len   = 0;

/* Ensure dynamic array capacities */
static void ensure_item_capacity(size_t need)
{
    if (items_cap >= need) return;
    size_t newcap = items_cap ? items_cap << 1 : 8;
    if (newcap < need) newcap = need;
    Item *newarr = (Item *)realloc(items, newcap * sizeof(Item));
    items = newarr;
    items_cap = newcap;
}

static void ensure_exp_capacity(idx_t need)
{
    if (exp_cap >= need) return;
    idx_t newcap = exp_cap ? (exp_cap << 1) : 8;
    if (newcap < need) newcap = need;
    seq_t *newex = (seq_t *)realloc(exponents, newcap * sizeof(seq_t));
    exponents = newex;
    exp_cap = newcap;
}

/* Generate terms up to at least n entries */
static void ensure_generated(idx_t n)
{
    if (items_len >= n) return;

    /* Initialize first term if empty */
    if (items_len == 0) {
        /* 6^0 * 10^0 = 1 */
        Item first;
        first.i = 0;
        first.j = 0;
        first.val = bigint_from_uint(1);
        ensure_item_capacity(1);
        items[0] = first;
        ensure_exp_capacity(1);
        exponents[0] = (seq_t)0;
        items_len = 1;
        exp_len = 1;
    }

    while (items_len < n) {
        /* Compute candidates */
        BigInt *prod6  = bigint_mul_small(items[idx6].val, 6);
        BigInt *prod10 = bigint_mul_small(items[idx10].val, 10);

        int cmp = bigint_cmp(prod6, prod10);

        Item next;
        if (cmp < 0) {                     /* prod6 < prod10 */
            next.i = items[idx6].i + 1;
            next.j = items[idx6].j;
            next.val = prod6;
            bigint_free(prod10);
            ++idx6;
        }
        else if (cmp > 0) {                /* prod10 < prod6 */
            next.i = items[idx10].i;
            next.j = items[idx10].j + 1;
            next.val = prod10;
            bigint_free(prod6);
            ++idx10;
        }
        else {                             /* equal */
            next.i = items[idx6].i + 1;
            next.j = items[idx6].j;
            next.val = prod6;              /* use one copy */
            bigint_free(prod10);           /* free the other */
            ++idx6; ++idx10;
        }

        /* Append new item */
        ensure_item_capacity(items_len + 1);
        items[items_len] = next;

        /* Store exponent result */
        ensure_exp_capacity(exp_len + 1);
        exponents[exp_len] = (seq_t)next.i;

        ++items_len;
        ++exp_len;
    }
}

/* Public function --------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not used, but safe */
    ensure_generated(n);
    return exponents[n - 1];
}
