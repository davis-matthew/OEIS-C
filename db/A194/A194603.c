/*
 * A194603  Smallest prime either of the form n*2^k - 1 or n*2^k + 1, k >= 0,
 *          or 0 if no such prime exists.
 * https://oeis.org/A194603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 240, u32 n <= 762, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Smallest prime of the form n*2^k ± 1 (k ≥ 0), or 0 if none exists. */
/* The function uses a straightforward search over increasing k. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

#define MAX_K 200  /* limit to avoid infinite loops if no prime is found */

seq_t a(idx_t n)
{
    /* Index starts at 1 */
    seq_t tmp = (seq_t)n;   /* n*2^k, starting with k=0 */

    for (idx_t k = 0;; ++k) {
        /* Process the current k */
        if (k > 0) {
            /* Multiply by 2, detect overflow for unsigned integer types */
            seq_t prev = tmp;
            tmp = tmp * (seq_t)2;
            if (tmp < prev)
                break;                    /* overflow → stop searching */
        }

        /* val1 = n*2^k - 1 (skip if <= 0) */
        if (tmp > (seq_t)1) {
            seq_t val1 = tmp - (seq_t)1;
            if (is_prime_big(val1))
                return val1;
        }

        /* val2 = n*2^k + 1 */
        seq_t val2 = tmp + (seq_t)1;
        if (is_prime_big(val2))
            return val2;

        /* Stop if we have exceeded our practical search limit */
        if (k >= MAX_K)
            break;
    }

    return (seq_t)0;  /* no prime found within the search limits */
}
