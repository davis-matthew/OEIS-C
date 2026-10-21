/*
 * A071196  The sum of the sequence starting with prime(n) and having prime
 *          sum defined in A071194, or -1 if no such sequence exists.
 * https://oeis.org/A071196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) is the sum of the first k consecutive primes starting with
 *            prime(n) * such that the total sum is prime and every earlier
 *            partial sum of * length > 1 is composite. If no such k exists,
 *            the function returns -1
 * Range:     u8 n <= 11, u16 n <= 159, u32 n <= 2200, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)-1;
    }

    seq_t sum = 0;
    /* We try adding primes until we find a prime sum or until a safe limit
     * is reached (to avoid infinite loops for sequences that might not exist).
     */
    for (idx_t step = 0; ; ++step) {
        uint32_t p = get_prime(n + step);   /* kth prime, 1‑based */
        sum += (seq_t)p;

        /* A valid sequence must have length at least 2, so we only test
         * primality after adding the second prime.
         */
        if (step >= 1) {
            if (is_prime_big(sum)) {
                return sum;                 /* found the required sum */
            }
        }

        /* Heuristic limit: if we have checked 2000 primes without success,
         * we assume no sequence exists and return -1.
         */
        if (step >= 2000) {
            return (seq_t)-1;
        }
    }
}
