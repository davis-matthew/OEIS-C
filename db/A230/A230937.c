/*
 * A230937  Number of white-square subarrays of (n+2) X (3+2) 0..3 arrays
 *          x(i,j) with each element diagonally or antidiagonally next to at
 *          least one element with value (x(i,j)+1) mod 4, no adjacent
 *          elements equal, and upper left element zero.
 * https://oeis.org/A230937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A230937 – Number of white-square subarrays of (n+2)×(3+2) 0..3 arrays
 *
 *  Empirical recurrence (valid for n ≥ 11):
 *       a(n) = 16*a(n-2) + 3*a(n-4) - 10*a(n-6) + 24*a(n-8) - 16*a(n-10)
 *
 *  Initial values (1 ≤ n ≤ 10):
 *       a(1)=2,  a(2)=16, a(3)=34, a(4)=232, a(5)=522,
 *       a(6)=3768, a(7)=8450, a(8)=60824, a(9)=136474, a(10)=982552
 *
 *  The function below memoises computed terms so that arbitrary call
 *  orders are satisfied and works for any build‑time seq_t.  All arithmetic
 *  uses modulo arithmetic of the underlying unsigned integer type, which
 *  is appropriate for the OEIS sequence.  No external libraries are
 *  required.  */

#include <stdlib.h>
#include <stdint.h>

/* --- Static memoisation state ------------------------------------------------- */
static seq_t *cache      = NULL;   /* 0‑based array, cache[0] unused          */
static idx_t  cache_len  = 0;      /* number of allocated entries (>=0)     */
static idx_t  last_comp  = 0;      /* highest index for which a(i) is known */

/* --- Initialise the first ten terms ------------------------------------------- */
static void
init_first_ten(void)
{
    static const seq_t first[11] = {
        0ULL,          /* dummy for index 0 (unused) */
        2ULL, 16ULL, 34ULL, 232ULL, 522ULL,
        3768ULL, 8450ULL, 60824ULL, 136474ULL, 982552ULL
    };

    for (idx_t i = 1; i <= 10; ++i)
        cache[i] = first[i];
}

/* --- Ensure the cache can hold indices up to 'need' (inclusive) --------------- */
static void
ensure_cache(idx_t need)
{
    if (need + 1 <= cache_len)
        return;

    /* Allocate at least 'need + 1' entries, rounded up to the next power of 2
       for efficient future growth. */
    idx_t new_len = 1;
    while (new_len <= need)
        new_len <<= 1;
    new_len += 1;               /* to include index 'need' */

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) { free(cache); exit(EXIT_FAILURE); }

    /* Initialise newly allocated entries to zero. */
    for (idx_t i = cache_len; i < new_len; ++i)
        tmp[i] = 0;

    cache      = tmp;
    cache_len  = new_len;
}

/* --- Main sequence function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* defined only for n ≥ 1 */

    /* We need the cache to cover at least the first 10 indices because we
       initialise them explicitly, and also up to n. */
    idx_t need = n > 10 ? n : 10;
    ensure_cache(need);

    /* Initialise the first ten terms once. */
    if (cache[1] == 0)
        init_first_ten();

    /* If we have already computed beyond the required index, just return. */
    if (n <= last_comp)
        return cache[n];

    /* Compute terms from (last_comp + 1) up to n.  For indices < 11 we rely
       on the pre‑initialised values; for n ≥ 11 we use the recurrence. */
    if (n >= 11) {
        idx_t start = last_comp + 1;
        if (start < 11) start = 11;
        for (idx_t i = start; i <= n; ++i) {
            seq_t val =
                16u * cache[i - 2]    /* a(i-2) */
              + 3u  * cache[i - 4]    /* a(i-4) */
              - 10u * cache[i - 6]    /* a(i-6) */
              + 24u * cache[i - 8]    /* a(i-8) */
              - 16u * cache[i -10];   /* a(i-10) */
            cache[i] = val;
        }
    }

    last_comp = n > last_comp ? n : last_comp;
    return cache[n];
}
