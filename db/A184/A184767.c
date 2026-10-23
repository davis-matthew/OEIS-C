/*
 * A184767  Number of (n+1)X4 0..3 arrays with every 2X2 subblock singular.
 * https://oeis.org/A184767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A184767: Number of (n+1)×4 0..3 arrays with every 2×2 subblock singular.
 *  Recurrence (order 7):
 *      a(1) = 2248
 *      a(2) = 108496
 *      a(3) = 1967440
 *      a(4) = 62051344
 *      a(5) = 1444491280
 *      a(6) = 39939558160
 *      a(7) = 1005763454224
 *      a(n) = 36·a(n‑1) – 51·a(n‑2) – 7976·a(n‑3) + 65448·a(n‑4)
 *            + 105984·a(n‑5) – 2142720·a(n‑6) + 4230144·a(n‑7)   (n ≥ 8)
 *
 *  The function memoizes values so that arbitrary ordering of calls is
 *  supported.  All arithmetic is performed with seq_t, which must
 *  support signed multiplication and subtraction.
 */

#include <stdlib.h>

/*  static state  */
static seq_t *cache = NULL;      /* array of computed terms (0‑based) */
static size_t capacity = 0;     /* current allocated size of cache   */
static idx_t   last = 0;        /* highest index that has been computed (0‑based) */

/*  ensure that cache has space for at least 'need' terms  */
static void ensure_cache(idx_t need)
{
    if (capacity >= (size_t)need) return;

    size_t newcap = capacity ? capacity << 1 : 8;
    while (newcap < (size_t)need) newcap <<= 1;

    seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – abort program (no other error handling needed) */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    capacity = newcap;
}

/*  initialize base terms  */
static void init_base(void)
{
    if (capacity < 7) ensure_cache(7);

    cache[0] = 2248;
    cache[1] = 108496;
    cache[2] = 1967440;
    cache[3] = 62051344;
    cache[4] = 1444491280;
    cache[5] = 39939558160;
    cache[6] = 1005763454224;
    last = 6;    /* last computed index (0‑based) */
}

/*  main sequence function  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* According to description, n≥1; return 0 for safety */

    if (!cache) init_base();      /* first call – set base values */

    if (n > capacity)             /* need more space */
        ensure_cache(n);

    /*  if the requested term is already computed, return it  */
    if (n - 1 <= last)
        return cache[n - 1];

    /*  compute missing terms up to index n-1  */
    for (idx_t k = last + 1; k < n; ++k) {
        /*  recurrence for k ≥ 7 (since k is 0‑based, k==7 corresponds to n==8)  */
        seq_t val =  36 * cache[k - 1]
                   - 51 * cache[k - 2]
                   - 7976 * cache[k - 3]
                   + 65448 * cache[k - 4]
                   + 105984 * cache[k - 5]
                   - 2142720 * cache[k - 6]
                   + 4230144 * cache[k - 7];
        cache[k] = val;
    }
    last = n - 1;

    return cache[n - 1];
}
