/*
 * A158023  Sides of squares which are filled exactly (no holes, no overlaps)
 *          by the digits needed to write a subsequence of consecutive
 *          nonnegative integers starting with 0.
 * https://oeis.org/A158023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sqrt(total_digits) where total_digits is the sum of
 *            decimal digits of the numbers 0,1,2,3,… until total_digits is a
 *            perfect square. a(n) is the n‑th such square root (OEIS A158023)
 * Range:     u8 n <= 47, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* -------------------------------------------------------------------------- */
/* helper: integer square root for uint64_t by binary search                 */
static inline uint64_t isqrt_uint64(uint64_t n)
{
    if (n < 2U)
        return n;
    uint64_t low = 1, high = n, mid, best = 1;
    while (low <= high) {
        mid  = low + ((high - low) >> 1);
        uint64_t sq = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n) {
            best = mid;
            low  = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

/* helper: number of decimal digits of a uint64_t number (fast loop)       */
static inline int digit_len_uint64(uint64_t x)
{
    int len = 1;
    while (x >= 10U) {
        x /= 10U;
        ++len;
    }
    return len;
}

/* -------------------------------------------------------------------------- */
/* persistent state for memoisation                                         */
static seq_t *seq_cache = NULL;   /* array of computed a(k) values   */
static size_t cache_cap = 0;      /* current capacity of seq_cache   */
static size_t cache_len = 0;      /* number of values stored         */
static idx_t  last_m     = (idx_t)-1; /* last integer processed          */
static uint64_t cum_digits = 0;   /* cumulative digit count          */

/* -------------------------------------------------------------------------- */
/* main function: return the n-th term of the sequence                      */
seq_t a(idx_t n)
{
    /* n must be >= 1 (offset 1).  Undefined behaviour for n == 0.        */
    if (n == 0U)
        return (seq_t)0;

    /* extend cache until we have at least n values                          */
    while (cache_len < (size_t)n) {
        /* advance to next integer in the natural number sequence           */
        ++last_m;
        cum_digits += (uint64_t)digit_len_uint64((uint64_t)last_m);

        /* check if cumulative digits form a perfect square                */
        uint64_t root = isqrt_uint64(cum_digits);
        if (root * root == cum_digits) { /* perfect square found            */
            if (cache_len >= cache_cap) { /* grow cache if necessary        */
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(seq_cache,
                                               cache_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failure – abort              */
                    abort();
                }
                seq_cache = tmp;
            }
            seq_cache[cache_len++] = (seq_t)root;
        }
    }

    return seq_cache[(size_t)n - 1];
}
