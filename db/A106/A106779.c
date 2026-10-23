/*
 * A106779  Primes with digit sum = 49.
 * https://oeis.org/A106779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2700, u64 n <= 2700, u128 n <= 2700,
 *            big any n
 * Helpers:   add_seq, digit_sum_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates primes with digit sum 49 by iteratively testing
   natural numbers and storing those that satisfy both conditions in a static
   array. It reuses the generated list for subsequent calls. */
#include "../utils/is_prime.h"
#include "../utils/digit_sum_uint64.h"
#include "../utils/add_seq.h"

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;
    static idx_t arr_cap = 0, arr_size = 0;
    static uint64_t cur = 1;

    while (arr_size < n) {
        cur++;
        if (is_prime(cur) && digit_sum_uint64(cur) == (seq_t)49) {
            add_seq(&arr, &arr_cap, &arr_size, (seq_t)cur);
        }
    }

    return arr[n - 1];
}
