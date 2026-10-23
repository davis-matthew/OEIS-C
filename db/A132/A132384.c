/*
 * A132384  a(n) = Sum_{ k <= n, k is not an i-th power with i >= 2} k.
 * https://oeis.org/A132384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A132384.
   a(n) = Σ_{k=1..n, k not a perfect i-th power (i≥2)} k
   Implemented with a cache of prefix sums that grows on demand.
   Perfect powers are detected by a simple power‑sieve. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Static cache of prefix sums */
static seq_t *prefix = NULL;        /* prefix[0] = 0, prefix[i] = Σ_{k=1..i} k′ */
static idx_t  max_seen = 0;         /* largest n for which prefix is valid */

/* Static cache of perfect–power flags */
static bool *is_pow = NULL;         /* is_pow[i] = true iff i is a perfect power */
static idx_t  pow_max = 0;          /* highest index for which is_pow is valid */

/* Build or extend the perfect‑power flag array up to max_n. */
static void build_pow_flags(idx_t max_n)
{
    if (pow_max >= max_n)
        return;                       /* already up to date */

    /* Allocate new array */
    idx_t new_size = max_n + 1;
    bool *new_arr = (bool *)realloc(is_pow, new_size * sizeof(bool));
    if (!new_arr)
        return;                       /* allocation failed – will be caught later */
    is_pow = new_arr;
    /* initially clear all entries from previous size to new size */
    for (idx_t i = pow_max + 1; i <= max_n; ++i)
        is_pow[i] = false;

    /* Mark perfect powers */
    for (idx_t base = 2; base * base <= max_n; ++base) {
        /* Use 128‑bit if available to avoid overflow when squaring */
        seq_t val = (seq_t)base * (seq_t)base;
        for ( /* exponent >= 2 */ ; val <= max_n; ) {
            is_pow[(idx_t)val] = true;
            if (val > max_n / base) break;   /* next multiplication would overflow or exceed */
            val = val * (seq_t)base;
        }
    }

    pow_max = max_n;
}

/* Main function required by the API */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* not used, but keeps the function defined for n=0 */

    /* Ensure the perfect‑power flag array is up to date */
    build_pow_flags(n);

    /* Grow the prefix array if needed */
    if (n > max_seen) {
        idx_t new_size = n + 1;          /* indices 0 .. n */
        seq_t *new_pref = (seq_t *)realloc(prefix, new_size * sizeof(seq_t));
        if (!new_pref)
            return (seq_t)0;             /* allocation failed – unlikely in tests */
        prefix = new_pref;
        if (max_seen == 0)
            prefix[0] = (seq_t)0;       /* base case */

        /* Compute new prefix values up to n */
        for (idx_t i = max_seen + 1; i <= n; ++i) {
            seq_t add = is_pow[i] ? (seq_t)0 : (seq_t)i;
            prefix[i] = prefix[i - 1] + add;
        }
        max_seen = n;
    }

    return prefix[n];
}
