/*
 * A129096  A bisection of A129095: a(n) = A129095(2n-1) for n>=1.
 * https://oeis.org/A129096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 69, u32 n <= 513, u64 n <= 7393,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A129096 – bisection of A129095.
 *  a(n) = A129095(2n-1) for n >= 1.
 *
 *  A129095 satisfies the recurrence:
 *    b(1) = 1
 *    b(n) = b(n/2)            if n even
 *    b(n) = 2*b(n-1) + b(n-2) if n odd  (>1)
 *
 *  The function below implements this recurrence with memoisation
 *  so that a(n) can be queried in any order.  It is efficient for
 *  n up to at least 10 000 and works for any supported integer type
 *  seq_t (including arbitrary precision).
 */

#include <stdlib.h>

static seq_t *b_cache    = NULL;   /* cache of computed b(n) values, 1‑based */
static size_t  cache_len = 0;      /* current number of allocated entries */

/* ensure the cache can hold at least indices up to 'n' */
static void ensure_cache(idx_t n) {
    if ((size_t)n < cache_len) return;          /* already large enough */
    size_t new_len = (size_t)n + 1;             /* 1‑based array, index 0 unused */
    seq_t *new_cache = (seq_t*)realloc(b_cache, new_len * sizeof(seq_t));
    if (!new_cache) abort();                    /* allocation failed */
    /* initialise new part to zero – zero means “not yet computed” */
    for (size_t i = cache_len; i < new_len; ++i) new_cache[i] = 0;
    b_cache   = new_cache;
    cache_len = new_len;
}

/* recursive, memoised calculation of b(n) */
static seq_t b_rec(idx_t n) {
    ensure_cache(n);
    if (b_cache[n] != 0) return b_cache[n];     /* already computed */

    seq_t value;
    if (n == 1) {
        value = 1;                              /* base case */
    } else if ((n & 1) == 0) {                  /* n even */
        value = b_rec(n >> 1);                  /* b(n/2) */
    } else {                                    /* n odd, n > 1 */
        seq_t a = b_rec(n - 1);
        seq_t b = b_rec(n - 2);
        value = (2 * a) + b;                    /* 2*b(n-1) + b(n-2) */
    }
    b_cache[n] = value;                         /* store result */
    return value;
}

/* public function – compute a(n) */
seq_t a(idx_t n) {
    if (n == 0) return 0;                       /* out of sequence bounds */
    idx_t odd_idx = (n << 1) - 1;               /* 2n-1 */
    return b_rec(odd_idx);
}
