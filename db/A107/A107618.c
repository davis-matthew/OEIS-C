/*
 * A107618  Primes with digit sum = 64.
 * https://oeis.org/A107618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   the nth prime whose digit sum is 64
 * Range:     u8 none, u16 none, u32 n <= 9789, u64 n <= 9789, u128 n <= 9789,
 *            big any n
 * Helpers:   digit_sum_uint64, is_prime, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum_uint64.h"
#include "../utils/is_prime.h"
#include "../utils/push_seq.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;   /* array of primes with digit sum 64 */
    static idx_t cap = 0;          /* current capacity of the array    */
    static idx_t size = 0;         /* number of primes discovered       */
    static uint64_t next = 3;      /* next odd number to test           */

    /* Find enough primes to satisfy the requested index */
    while (size < n) {
        uint64_t cand = next;
        next += 2;                  /* iterate over odd numbers only */

        if (digit_sum_uint64(cand) == 64 && is_prime(cand)) {
            push_seq(&primes, &cap, &size, (seq_t)cand);
        }
    }

    return primes[n - 1];
}
