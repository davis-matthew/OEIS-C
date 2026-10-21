/*
 * A068951  Scan the primes, record digit-sum if it is itself prime.
 * https://oeis.org/A068951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = digit_sum(p_n) where p_n is the n‑th prime and
 *            digit_sum(p_n) is prime. (n starts at 1)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   digit_sum_uint32, grow_seq_t_arr, is_small_prime_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum_uint32.h"
#include "../utils/grow_seq_t_arr.h"
#include "../utils/is_small_prime_int.h"

/* ---- small helpers --------------------------------------------------- */

/* primality test for primes that appear in the sequence.
   The p_n we need for n ≤ 10000 are below 200000, fit in uint32_t. */
static bool is_prime_uint32(uint32_t n)
{
    if (n < 2)            return false;
    if (n == 2)           return true;
    if ((n & 1) == 0)     return false;
    for (uint32_t p = 3; (uint64_t)p * p <= n; p += 2)
        if (n % p == 0)   return false;
    return true;
}

/* ---- sequence implementation ------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 0 */

    static seq_t *vals      = NULL;
    static size_t vals_len  = 0;
    static size_t vals_cap  = 0;
    static uint32_t next_candidate = 2;   /* next prime to try */

    while (vals_len < n) {
        /* find next prime starting from next_candidate */
        uint32_t p;
        for (p = next_candidate; ; ++p) {
            if (is_prime_uint32(p)) break;
        }
        next_candidate = p + 1;           /* cache for next call */

        /* digit sum */
        int ds = digit_sum_uint32(p);
        if (is_small_prime_int(ds)) {
            if (vals_len >= vals_cap) grow_seq_t_arr(&vals, &vals_cap);
            vals[vals_len++] = (seq_t)ds;
        }
    }
    return vals[n - 1];
}
