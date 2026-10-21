/*
 * A062384  Positive numbers whose product of digits is 7 times their sum.
 * https://oeis.org/A062384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 117, u32 n <= 500, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A062384 – Positive numbers whose product of digits is 7 times their sum.
 *
 * The function below generates the sequence by scanning integers in ascending
 * order and selecting those that satisfy: prod(digits(n)) = 7 * sum(digits(n)).
 * Results are cached so that subsequent calls retrieve already computed terms.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *a_cache = NULL;        /* cache of computed terms */
static idx_t a_len   = 0;            /* number of terms in cache   */
static idx_t a_cap   = 0;            /* allocated capacity         */
static uint64_t next_start = 1;      /* next integer to examine   */

/* Ensure cache can hold at least 'need' terms */
static void a_ensure_capacity(idx_t need) {
    if (need <= a_cap) return;
    idx_t new_cap = a_cap ? a_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return;         /* allocation failure – unlikely in this context */
    if (a_cache) {
        memcpy(new_cache, a_cache, a_len * sizeof(seq_t));
        free(a_cache);
    }
    a_cache = new_cache;
    a_cap   = new_cap;
}

/* Append a new term to the cache */
static void a_append(uint64_t val) {
    a_ensure_capacity(a_len + 1);
    a_cache[a_len++] = (seq_t)val;
    next_start = val + 1;
}

/* Check whether a positive integer has digit product = 7 * digit sum.
 * Returns 0 for false, non‑zero for true. */
static int satisfies(uint64_t x) {
    seq_t prod = 1;
    seq_t sum  = 0;
    while (x > 0) {
        unsigned digit = x % 10;
        if (digit == 0) return 0;   /* product would be 0, impossible unless sum=0 */
        prod *= (seq_t)digit;
        sum  += (seq_t)digit;
        x /= 10;
    }
    return prod == 7 * sum;
}

/*
 * Compute a(n) for n ≥ 1.
 *
 * The function scans successive integers, storing those that meet the
 * product‑sum condition.  Results are cached so later calls are O(1)
 * (aside from the required cache size).
 */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* undefined for n < 1; return 0 as safeguard */

    /* Grow cache until it contains the requested term */
    while (a_len < n) {
        uint64_t candidate = next_start;
        /* Scan forward until a valid number is found */
        while (1) {
            if (satisfies(candidate)) {
                a_append(candidate);
                break;
            }
            /* Prevent unsigned wrap‑around on the very unlikely case
             * that next_start reaches maximum uint64_t value. */
            if (candidate == UINT64_MAX) {
                /* In the extreme unlikely event that the required term
                 * cannot be found within 64‑bit space, future calls will
                 * return 0 for remaining indices. */
                a_append(0);
                break;
            }
            ++candidate;
        }
    }

    return a_cache[n - 1];
}
