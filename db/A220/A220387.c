/*
 * A220387  Majority value maps: number of 2Xn binary arrays indicating the
 *          locations of corresponding elements equal to at least half of
 *          their horizontal and antidiagonal neighbors in a random 0..1 2Xn
 *          array.
 * https://oeis.org/A220387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the empirical linear recurrence * a(n) = 3*a(n-1)
 *            +4*a(n-3) +7*a(n-4) -7*a(n-6) -19*a(n-7) * -21*a(n-8) -33*a(n-9)
 *            -14*a(n-10) +a(n-11) +6*a(n-12) * +45*a(n-13) +27*a(n-14)
 *            +72*a(n-15) -a(n-16) +6*a(n-17) * -32*a(n-18) -16*a(n-19)
 *            -4*a(n-20) +4*a(n-22) * with initial values a(1) … a(...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;   /* 1‑based array of values */
static idx_t  capacity = 0;   /* size of allocated array minus 1 */
static idx_t  computed = 0;   /* highest n computed so far (0 if none) */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n <= capacity)
        return;
    idx_t new_cap = n + 1;            /* indices 0 … n */
    seq_t *new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (!new_cache)            /* out of memory – terminate */
        abort();
    if (cache) {
        memcpy(new_cache, cache, (capacity + 1) * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    capacity = new_cap - 1;
}

/* Initial 24 terms of the sequence, 1‑based. a(0) is unused (0). */
static const seq_t init_vals[] = {
    0ULL,                      /* dummy for index 0 */
    1ULL, 7ULL, 25ULL, 79ULL, 278ULL, 966ULL, 3362ULL,
    11642ULL, 40375ULL, 140102ULL, 486191ULL, 1686966ULL,
    5853300ULL, 20309556ULL, 70470311ULL, 244518242ULL,
    848429087ULL, 2943877082ULL, 10214660764ULL, 35442822154ULL,
    122979474647ULL, 426714063696ULL,
    1480612045993ULL, 5137426278616ULL
};
static const size_t init_count = sizeof(init_vals) / sizeof(init_vals[0]) - 1; /* 24 */

/* Compute a(n) and cache all intermediate values up to n. */
seq_t a(idx_t n)
{
    if (n == 0)  /* sequence is defined for n >= 1 */
        return (seq_t)0;

    if (n <= init_count)      /* use the predefined values */
        return init_vals[n];

    ensure_capacity(n);       /* guarantee storage for 1…n */

    /* Initialize the cache with the known first 24 terms if not yet done. */
    if (computed < init_count) {
        for (idx_t i = 1; i <= init_count; ++i)
            cache[i] = init_vals[i];
        computed = init_count;
    }

    /* Compute missing terms using the empirical recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t pos = 3 * cache[i - 1];
        pos += 4 * cache[i - 3];
        pos += 7 * cache[i - 4];
        pos += 1 * cache[i - 11];
        pos += 6 * cache[i - 12];
        pos += 45 * cache[i - 13];
        pos += 27 * cache[i - 14];
        pos += 72 * cache[i - 15];
        pos += 6 * cache[i - 17];
        pos += 4 * cache[i - 22];

        seq_t neg = 7 * cache[i - 6];
        neg += 19 * cache[i - 7];
        neg += 21 * cache[i - 8];
        neg += 33 * cache[i - 9];
        neg += 14 * cache[i - 10];
        neg += 1 * cache[i - 16];
        neg += 32 * cache[i - 18];
        neg += 16 * cache[i - 19];
        neg += 4 * cache[i - 20];

        cache[i] = pos - neg;   /* the result is non‑negative for all n≥1 */
    }

    computed = n;
    return cache[n];
}
