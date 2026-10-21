/*
 * A235452  Take the union of all the sequences Collatz(i) for i <= n. The
 *          number a(n) is the largest of consecutive numbers beginning with
 *          1.
 * https://oeis.org/A235452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A235452: a(n) is the largest k such that {1,…,k}
   is contained in the union of the Collatz sequences of all i ≤ n. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* Dynamic bitset: one bit per integer index */
typedef struct {
    uint64_t *words;   /* words of 64 bits */
    idx_t    len;      /* number of allocated words */
} BitSet;

/* Ensure that index idx can be represented */
static void bs_ensure(BitSet *bs, idx_t idx)
{
    idx_t word = idx >> 6;                /* idx / 64 */
    if (word >= bs->len) {
        idx_t new_len = bs->len ? bs->len << 1 : 16;
        while (new_len <= word) new_len <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(bs->words,
                                            new_len * sizeof(uint64_t));
        if (!tmp) return;                 /* allocation failed – ignore */
        if (bs->words) {
            memset(tmp + bs->len, 0,
                   (new_len - bs->len) * sizeof(uint64_t));
        } else {
            memset(tmp, 0, new_len * sizeof(uint64_t));
        }
        bs->words = tmp;
        bs->len   = new_len;
    }
}

/* Set bit at position idx */
static void bs_set(BitSet *bs, idx_t idx)
{
    if (idx == 0) return;                 /* we never need 0 */
    bs_ensure(bs, idx);
    bs->words[idx >> 6] |= 1ULL << (idx & 63);
}

/* Test if bit at position idx is set */
static bool bs_get(const BitSet *bs, idx_t idx)
{
    if (idx == 0) return false;
    idx_t word = idx >> 6;
    if (word >= bs->len) return false;
    return (bs->words[word] >> (idx & 63)) & 1ULL;
}

/* Free bitset resources */
static void bs_free(BitSet *bs)
{
    free(bs->words);
    bs->words = NULL;
    bs->len   = 0;
}

/* Compute a(n) for A235452 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    BitSet bs = {NULL, 0};

    /* Iterate over all starting values 1..n */
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t x = (uint64_t)i;          /* use 64‑bit for safety */
        while (1) {
            bs_set(&bs, (idx_t)x);
            if (x == 1) break;
            if ((x & 1ULL) == 0) {
                x >>= 1;
            } else {
                x = x * 3ULL + 1ULL;
            }
        }
    }

    /* Find longest consecutive run starting at 1 */
    idx_t k = 0;
    while (bs_get(&bs, k + 1)) ++k;

    bs_free(&bs);
    return (seq_t)k;
}
