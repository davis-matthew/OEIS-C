/*
 * A071122  a(n) = a(n-1) + sum of decimal digits of 2^n.
 * https://oeis.org/A071122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = a(n-1) + sum of decimal digits of 2^n. The initial value
 *            is a(1) = 2
 * Range:     u8 n <= 18, u16 n <= 310, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Need allocation utilities for dynamic arrays. */
#include <stdlib.h>

/*======================================================================
 * Efficient computation for any n ≥ 1.
 * The routine keeps a cache of previously computed a(k) values
 * and the decimal representation of 2^k in LSD‑first order.
 * Subsequent calls in any order are O(1) for the cached part
 * plus an O(9·digits(2^k)) update for new terms.
 *====================================================================*/

seq_t a(idx_t n)
{
    /*--------------------------------------------------------------
     * Cache of sequence values a(0..cached_n).  a(0) is unused
     * but convenient to have an index 0 element.
     *--------------------------------------------------------------*/
    static seq_t *seq_cache = NULL;      /* array of seq_t values */
    static idx_t  cache_size = 0;        /* allocated size (≥ cached_n+1) */
    static idx_t  cached_n   = 0;        /* largest index stored */

    /*--------------------------------------------------------------
     * Decimal representation of 2^k (k = cached_n), LSD first.
     * Each digit is a byte 0..9.
     *--------------------------------------------------------------*/
    static uint8_t *pow_digits = NULL;   /* array of digits */
    static size_t  pow_len   = 0;        /* number of used digits */
    static size_t  pow_cap   = 0;        /* allocated capacity */

    /*--------------------------------------------------------------
     * Guard against the forbidden offset n == 0.
     * For the OEIS definition the sequence starts at n = 1.
     *--------------------------------------------------------------*/
    if (n == 0) return 0;

    /*--------------------------------------------------------------
     * Initialise static structures on first entry.
     *--------------------------------------------------------------*/
    if (seq_cache == NULL) {
        cache_size = n + 1;
        seq_cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!seq_cache) return 0; /* out‑of‑memory; silently return 0 */
        seq_cache[0] = 0;          /* dummy value for a(0) */
        cached_n = 0;
    } else if (cache_size <= n) {   /* enlarge cache if necessary */
        size_t new_size = cache_size * 2;
        if (new_size <= n) new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;        /* out‑of‑memory; silently return 0 */
        seq_cache = tmp;
        cache_size = new_size;
    }

    /*--------------------------------------------------------------
     * Initialise the power representation for 2^0.
     *--------------------------------------------------------------*/
    if (pow_digits == NULL) {
        pow_cap = 64;
        pow_digits = (uint8_t*)malloc(pow_cap * sizeof(uint8_t));
        if (!pow_digits) return 0; /* out‑of‑memory */
        pow_digits[0] = 1;       /* 2^0 == 1 */
        pow_len = 1;
    }

    /*--------------------------------------------------------------
     * Incrementally compute terms up to n if we do not have them.
     * Each step multiplies the current decimal number by 2 and
     * updates the digit sum, then stores the new a(k).
     *--------------------------------------------------------------*/
    if (cached_n < n) {
        for (idx_t k = cached_n + 1; k <= n; ++k) {
            /* Multiply pow_digits by 2 (in-place). */
            unsigned carry = 0, prod;
            for (size_t i = 0; i < pow_len; ++i) {
                prod = (unsigned)pow_digits[i] * 2 + carry;
                pow_digits[i] = (uint8_t)(prod % 10);
                carry = prod / 10;
            }
            while (carry) {
                if (pow_len == pow_cap) {   /* grow array */
                    size_t new_cap = pow_cap * 2;
                    uint8_t *tmp = (uint8_t*)realloc(pow_digits, new_cap * sizeof(uint8_t));
                    if (!tmp) return 0;
                    pow_digits = tmp;
                    pow_cap = new_cap;
                }
                pow_digits[pow_len++] = (uint8_t)(carry % 10);
                carry /= 10;
            }

            /* Compute digit sum of the new power. */
            uint32_t sum = 0;
            for (size_t i = 0; i < pow_len; ++i)
                sum += (uint32_t)pow_digits[i];

            /* Update cache. */
            seq_cache[k] = seq_cache[k-1] + (seq_t)sum;
        }
        cached_n = n;
    }

    return seq_cache[n];
}
