/*
 * A135354  a(0)=1, a(n) = largest divisor of n! that is coprime to a(n-1).
 * https://oeis.org/A135354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0)=1; for n>0 a(n) is the largest divisor of n! that is
 *            coprime to a(n-1). For even n=2k, a(n) is the largest power of
 *            two dividing (2k)!. For odd n=2k+1, a(n) is the largest odd
 *            divisor of (2k+1)!, i.e. the product of all odd parts of the
 *            numbers from 1 to n
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation note:  We cache all values up to the largest n that has
    been requested so far.  Each new value is built from the previous
    one by multiplying the odd part of the current n and updating the
    power‑of‑two factor for even n.  The cache is only grown when a
    larger n is requested and works for any build that defines seq_t
    and idx_t. */

#include <stdlib.h>
#include <stdint.h>
/* If C++ is used, enclose declarations in extern "C" if needed. */

/* static state for memoisation */
static seq_t *cache = NULL;          /* sequence cache                     */
static idx_t cache_size = 0;         /* allocated cache slots              */
static idx_t cur_max = 0;            /* greatest index that has been cached */
static seq_t pow2_factor = 1;        /* product of 2^v2 of factorial so far */
static seq_t odd_product = 1;        /* product of odd parts so far         */

/* Helper: initialise cache on first call */
static void init_cache(void)
{
    /* Allocate space for a(0) */
    cache = (seq_t *)malloc(sizeof(seq_t));
    if (!cache) return;          /* allocation failure – should never happen in tests */
    cache[0] = (seq_t)1;
    cache_size = 1;
    cur_max = 0;
    pow2_factor = (seq_t)1;
    odd_product = (seq_t)1;
}

seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (!cache) init_cache();

    /* Return already computed value if available */
    if (n < cur_max) return cache[n];
    if (n == cur_max) return cache[n];

    /* Grow the cache if necessary */
    if (n + 1 > cache_size) {
        size_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failure */
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute terms from cur_max+1 up to n inclusive */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        /* trailing zeros of i give exponent of 2 in i */
        int tz = __builtin_ctzll((unsigned long long)i);      /* i is uint64_t */
        seq_t odd_part = (seq_t)(i >> tz);                    /* odd component of i */

        /* Update running products */
        odd_product *= odd_part;
        if ((i & 1ULL) == 0ULL)    /* even index */
            pow2_factor <<= tz;

        /* Store the required value for this index */
        cache[i] = ((i & 1ULL) == 0ULL) ? pow2_factor : odd_product;
    }

    cur_max = n;
    return cache[n];
}
