/*
 * A106786  Primes whose sum of digits is 59.
 * https://oeis.org/A106786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   add_seq, digit_sum_uint64, is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n‑th prime whose decimal digit sum is 59. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime64.h"
#include "../utils/digit_sum_uint64.h"
#include "../utils/add_seq.h"

/* Memoised storage of the primes found so far. */
static seq_t *prime_list = NULL;
static idx_t prime_cap   = 0;
static idx_t prime_cnt   = 0;
static uint64_t last_checked = 1;

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive n – behaviour below the offset is irrelevant. */
    if (n == 0)
        return (seq_t)0;

    /* Keep searching until we have at least n values stored. */
    while (prime_cnt < n)
    {
        ++last_checked;                           /* Next integer to examine */
        if (is_prime64(last_checked))
        {
            if (digit_sum_uint64(last_checked) == 59)
            {
                add_seq(&prime_list, &prime_cap, &prime_cnt,
                        (seq_t)last_checked);
            }
        }
    }
    return prime_list[n - 1];
}
