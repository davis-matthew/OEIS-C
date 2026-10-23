/*
 * A044185  Numbers n such that string 6,5 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose base‑7 representation
 *            contains the consecutive digits 6,5 but whose n‑1 representation
 *            does not. The function builds the sequence on demand and caches
 *            results
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define CACHE_MAX 200000UL  /* enough for n <= 10000 (empirical) */

/* Return true iff x (>= 0) in base 7 contains the substring 65 */
static bool has65(seq_t x)
{
    if (x == (seq_t)0)
        return false;

    int digits[32];   /* base‑7 has at most 22 digits for values < 2^64 */
    size_t len = 0;
    seq_t tmp = x;

    while (tmp != (seq_t)0) {
        digits[len++] = (int)(tmp % (seq_t)7);
        tmp /= (seq_t)7;
    }

    for (size_t i = len; i > 1; --i) {
        if (digits[i - 1] == 6 && digits[i - 2] == 5)
            return true;
    }
    return false;
}

/* Cached sequence storage */
static seq_t   cache[CACHE_MAX];
static size_t  cache_size   = 0;
static seq_t  next_to_test  = (seq_t)1;

/* Generate terms until we have at least n terms cached */
static void grow_cache(size_t n)
{
    while (cache_size < n) {
        seq_t val = next_to_test++;
        if (has65(val) && !has65(val - (seq_t)1)) {
            if (cache_size >= CACHE_MAX)
                abort();          /* safety: should never happen */
            cache[cache_size++] = val;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;           /* undefined for indices < 1 */

    grow_cache((size_t)n);
    return cache[n - 1];
}
