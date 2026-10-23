/*
 * A049960  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 3, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n-1 <=
 *          2^(p+1), with a(1) = 1 and a(2) = 2.
 * https://oeis.org/A049960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(A006257(n-2)) + Sum_{i=1}^{n-1} a(i) (n≥3) where
 *            A006257(k) = 2k – 2^{⌊log₂(k)⌋+1}, with a(1)=1, a(2)=2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are typedefs supplied by the build. */

#include <stdlib.h>
#include <stdint.h>

/* Static caches for computed values and prefix sums.  Index 0 is unused
   for a_cache but is stored as 0 in pref_cache to simplify sums. */
static seq_t *a_cache = NULL;
static seq_t *pref_cache = NULL;
/* cache_cap = number of allocated elements (>=1), pref_cache[0]..pref_cache[cache_cap-1] */
static idx_t cache_cap = 0;
/* cache_len = highest index for which a_cache[idx] has a valid value.  */
static idx_t cache_len = 0;

/* Compute ⌊log₂(x)⌋ for unsigned 64‑bit integers. */
static idx_t floor_log2_u64(idx_t x)
{
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

/* Grow the cache arrays to accommodate indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_cap) return;          /* we already have space */
    idx_t new_cap = n + 1;                    /* indices 0 .. n */
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    pref_cache = (seq_t *)realloc(pref_cache, new_cap * sizeof(seq_t));
    if (!a_cache || !pref_cache) abort();
    cache_cap = new_cap;
}

/* Return a(n) as defined above.  Supports calls in arbitrary order. */
seq_t a(idx_t n)
{
    /* Initialise the base cases on the first call. */
    if (cache_len == 0) {
        ensure_capacity(2);
        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)2;
        pref_cache[0] = (seq_t)0;
        pref_cache[1] = a_cache[1];
        pref_cache[2] = pref_cache[1] + a_cache[2];
        cache_len = 2;
    }

    if (n <= cache_len) return a_cache[n];

    ensure_capacity(n);

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Compute m = A006257(i-2) = 2*i - 3 - 2^{p+1} where
           p = ⌊log₂(i-2)⌋. */
        idx_t p = floor_log2_u64(i - 2);
        idx_t power = ((idx_t)1) << (p + 1);   /* 2^{p+1} */
        idx_t m = 2 * i - 3 - power;           /* 1 ≤ m < i */

        a_cache[i] = pref_cache[i - 1] + a_cache[m];
        pref_cache[i] = pref_cache[i - 1] + a_cache[i];
    }

    cache_len = n;
    return a_cache[n];
}
