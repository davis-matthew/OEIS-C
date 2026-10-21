/*
 * A240756  Number of n X 2 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or zero plus the sum of elements above
 *          it or zero plus the sum of the elements diagonally to its
 *          northwest or zero plus the sum of the elements antidiagonally to
 *          its northeast, modulo 4.
 * https://oeis.org/A240756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-3) + a(n-5) - 2*a(n-8) - 4*a(n-9) - a(n-11) +
 *            2*a(n-14) for n>17 Initial terms (1‑based): a(1)=5 a(2)=6
 *            a(3)=12 a(4)=16 a(5)=16 a(6)=35 a(7)=35 a(8)=36 a(9)=65 a(10)=83
 *            a(11)=102 a(12)=172 a(13)=191 a(14)=230 a(15)=381 a(16)=458
 *            a(17)=576
 * Range:     u8 n <= 14, u16 n <= 33, u32 n <= 72, u64 n <= 151,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses only the operators +, -, * and arithmetic with
   integer constants, so it works for native unsigned integer types as
   well as for the arbitrary‑precision seq_t class.  All values are
   memoised in a growable cache; the function is safe for random order
   access. */

#include <stdlib.h>

#define NUM_INIT 17   /* number of seeded values (indices 1..17) */

/* 17 initial terms */
static const seq_t init_vals[NUM_INIT] = {
    5,  6,  12, 16, 16,
    35, 35, 36, 65, 83,
    102,172,191,230,381,
    458,576
};

static seq_t *cache = NULL;      /* 1‑based cache, cache[0] unused */
static idx_t  cache_cap = 0;     /* allocated size (including idx 0) */
static idx_t  computed   = 0;    /* largest n for which cache[n] is valid */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : (cache_cap << 1);
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!new_cache) abort();   /* allocation failure */
        cache      = new_cache;
        cache_cap  = new_cap;
    }
}

/* Main sequence access function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n<1, just return 0 */

    ensure_cache(n);

    /* Copy initial values into cache if not already done. */
    if (computed < NUM_INIT) {
        for (idx_t k = 1; k <= NUM_INIT; ++k) cache[k] = init_vals[k-1];
        computed = NUM_INIT;
    }

    /* Compute any missing values up to n. */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t val;
        if (k <= NUM_INIT) {
            val = init_vals[k-1];
        } else {
            /* The recurrence can be regrouped to avoid negative temporaries: */
            /* a(k) = (3*a(k-3) + a(k-5) + 2*a(k-14)) - (2*a(k-8) + 4*a(k-9) + a(k-11)) */
            seq_t a3  = cache[k-3];
            seq_t a5  = cache[k-5];
            seq_t a8  = cache[k-8];
            seq_t a9  = cache[k-9];
            seq_t a11 = cache[k-11];
            seq_t a14 = cache[k-14];

            seq_t sum_pos = 3 * a3 + a5 + 2 * a14;
            seq_t sum_neg = 2 * a8 + 4 * a9 + a11;
            val = sum_pos - sum_neg;
        }
        cache[k] = val;
    }

    computed = n;
    return cache[n];
}
