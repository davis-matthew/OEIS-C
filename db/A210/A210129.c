/*
 * A210129  Number of (n+1) X 4 0..3 arrays with every 2 X 2 subblock having
 *          one, three or four distinct values, and new values 0..3 introduced
 *          in row major order.
 * https://oeis.org/A210129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by the 11‑term linear recurrence
 *            a(n)=95*a(n-1)-a(n-2)-14266*a(n-3)+20367*a(n-4)+435937*a(n-5)
 *            -1093869*a(n-6)-1923590*a(n-7)+8479272*a(n-8)-8472184*a(n-9)
 *            +1936944*a(n-10)+631296*a(n-11) with a(1)=897, a(2)=84381, …,
 *            a(11)=45539680606576639531819
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_from_str.h"

/* cache for computed terms; indices are 1‑based */
static seq_t *cache = NULL;
static size_t cap   = 0;
static idx_t  last  = 0;      /* highest computed index */

/* term strings for a(1)…a(11) */
static const char *terms[11] = {
    "897",
    "84381",
    "7873809",
    "735336694",
    "68668190865",
    "6412512897051",
    "598825892889354",
    "55920742053335527",
    "5222101117981021055",
    "487660555133397778704",
    "45539680606576639531819"
};

/* ensure that cache can hold indices up to n (1‑based) */
static void
ensure_cache_space(idx_t n)
{
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = cap ? cap << 1 : 32;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }
}

/* initialise the first 11 known terms */
static void
init_first_terms(void)
{
    if (last >= 11) return;          /* already done */
    ensure_cache_space(11);
    for (idx_t i = 1; i <= 11; ++i) {
        cache[i] = seq_from_str(terms[i - 1]);
    }
    last = 11;
}

/* public function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* out of range, return 0 */

    init_first_terms();              /* guarantees cache[1…11] */
    ensure_cache_space(n);

    if (n <= last)                  /* already computed */
        return cache[n];

    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term =
            ((seq_t)95      * cache[i - 1])          /* 95*a(n-1) */
          - ((seq_t)1      * cache[i - 2])          /* -a(n-2) */
          - ((seq_t)14266 * cache[i - 3])          /* -14266*a(n-3) */
          + ((seq_t)20367 * cache[i - 4])          /* +20367*a(n-4) */
          + ((seq_t)435937 * cache[i - 5])         /* +435937*a(n-5) */
          - ((seq_t)1093869 * cache[i - 6])        /* -1093869*a(n-6) */
          - ((seq_t)1923590 * cache[i - 7])        /* -1923590*a(n-7) */
          + ((seq_t)8479272 * cache[i - 8])        /* +8479272*a(n-8) */
          - ((seq_t)8472184 * cache[i - 9])        /* -8472184*a(n-9) */
          + ((seq_t)1936944 * cache[i -10])        /* +1936944*a(n-10) */
          + ((seq_t)631296  * cache[i -11]);       /* +631296*a(n-11) */
        cache[i] = term;
    }
    last = n;
    return cache[n];
}
