/*
 * A051247  Binary numbers d(1)...d(j) such that d(i) = d(j+1-i) for all but 4
 *          values of i.
 * https://oeis.org/A051247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose binary representation *
 *            differs from its bitwise reverse in exactly four bit positions.
 *            * Equivalently, the Hamming distance between the binary string
 *            of a(n) * and its reverse is 4. The sequence is listed in
 *            ascending order
 * Range:     u8 n <= 90, u16 n <= 9722, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* small utility functions ------------------------------------------------- */

/* len = number of bits needed to represent x (at least 1) */
static unsigned
bitlen_seq(seq_t x)
{
    unsigned len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len ? len : 1;
}

/* popcount for an arbitrary seq_t value, implemented by repeated shifts */
static unsigned
popcount_seq(seq_t x)
{
    unsigned cnt = 0;
    while (x) {
        if (x & 1) ++cnt;
        x >>= 1;
    }
    return cnt;
}

/* returns the value obtained by reversing the lowest `len` bits of x */
static seq_t
reverse_bits(seq_t x, unsigned len)
{
    seq_t rev = 0;
    for (unsigned i = 0; i < len; ++i) {
        if (x & 1)
            rev |= ((seq_t)1 << (len - 1 - i));
        x >>= 1;
    }
    return rev;
}

/* ------------------------------------------------------------------ */
/* cache for previously found terms ------------------------------------- */

static seq_t *cache      = NULL;   /* array of a(i) values, 0‑based */
static idx_t cache_size = 0;      /* number of terms currently stored   */
static idx_t cache_cap  = 0;      /* capacity of the allocation         */

static void
ensure_cache(idx_t need)
{
    if (cache_cap >= need)
        return;
    idx_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* public entry point ----------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)        /* offset is 1, but return 0 for safety */
        return 0;

    /* already computed? */
    if (cache_size >= n)
        return cache[n - 1];

    /* start searching from the first integer greater than the last known term */
    idx_t candidate = (cache_size == 0) ? 1 : ((idx_t)cache[cache_size - 1]) + 1;

    while (cache_size < n) {
        seq_t val = (seq_t)candidate;
        unsigned len = bitlen_seq(val);
        seq_t rev = reverse_bits(val, len);
        seq_t diff = val ^ rev;

        if (popcount_seq(diff) == 4) {
            /* store the found term */
            ensure_cache(cache_size + 1);
            cache[cache_size] = val;
            ++cache_size;
        }
        /* move to the next candidate integer */
        ++candidate;
    }
    return cache[n - 1];
}
