/*
 * A144360  Primes of the form 8^k + 7. Also, primes of the form 64^m + 7.
 * https://oeis.org/A144360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes of the form 64^m + 7 (m ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   is_prime_big, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    static seq_t *primes   = NULL;
    static size_t  primes_cnt = 0;
    static size_t  primes_cap = 0;
    static seq_t  current_power = 0;
    static bool   initialized  = false;

    if (!initialized) {
        /* initial buffer for primes */
        primes_cap   = 16;
        primes       = (seq_t *)malloc(primes_cap * sizeof(seq_t));
        primes_cnt   = 0;
        /* start with m = 1 : 64^1 */
        current_power = mul_small((seq_t)1, 64);
        initialized  = true;
    }

    /* generate primes until we have at least n of them */
    while (primes_cnt < n) {
        /* candidate = 64^m + 7 */
        seq_t candidate = current_power;
        candidate += (seq_t)7;

        if (is_prime_big(candidate)) {
            /* store the prime */
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap * 2;
                seq_t *new_arr = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!new_arr) abort();
                primes     = new_arr;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = candidate;
        }

        /* advance to next m : multiply by 64 to get 64^(m+1) */
        current_power = mul_small(current_power, 64);
    }

    return primes[n - 1];
}
