/*
 * A144586  a(n) = odd and nonprime sum (with multiplicity) of prime factors
 *          of composite(k) = A002808(k).
 * https://oeis.org/A144586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, is_prime, is_prime_big, seq_alloc,
 *            seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A144586: a(n) = sum of prime factors (with multiplicity) of the n‑th
 * composite number, when that sum is an odd composite number. The sequence
 * is built by iterating composite numbers and caching results. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Cache of the sequence values */
    static seq_t *cached   = NULL;
    static size_t cap      = 0, len = 0;
    static uint64_t next_comp = 4;
    static bool initialized = false;

    if (n == 0)
        return (seq_t)0;                     /* sequence is undefined at 0 */

    if (!initialized) {
        cap = 16;
        cached = seq_alloc(cap);             /* allocates an array of seq_t */
        if (!cached)
            return (seq_t)0;
        len = 0;
        next_comp = 4;
        initialized = true;
    }

    while (len < n) {
        /* Find the next composite number */
        while (is_prime(next_comp))
            ++next_comp;
        uint64_t N = next_comp;

        /* Sum of prime factors (with multiplicity) */
        seq_t sum = (seq_t)0;
        uint64_t tmp = N;
        while (tmp > 1) {
            uint64_t p = _smallest_prime_factor(tmp);
            if (p == 0)                        /* defensive, should not happen */
                break;
            sum += (seq_t)p;
            tmp /= p;
        }

        /* Check for odd composite sum */
        if ((sum % (seq_t)2) == (seq_t)1 && sum > (seq_t)1 && !is_prime_big(sum)) {
            /* Store the term, growing the cache if necessary */
            if (len + 1 > cap) {
                size_t newcap = cap * 2;
                seq_t *tmp_arr = seq_realloc(cached, cap, newcap);
                if (!tmp_arr)
                    return (seq_t)0;
                cached = tmp_arr;
                cap = newcap;
            }
            cached[len++] = sum;
        }

        /* Advance to the next integer for the next iteration */
        ++next_comp;
    }

    return cached[n - 1];
}
