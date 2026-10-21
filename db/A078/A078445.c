/*
 * A078445  Primes in A060620, i.e., primes which are integer parts of
 *          averages of initial primes.
 * https://oeis.org/A078445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sum_{i=1..k} prime(i)/k) for all k with this floor
 *            being prime. This implementation generates terms in ascending
 *            order by scanning k from 1 upward and recording each floor
 *            average that is prime. Each call computes the n‑th term from
 *            scratch; the cost is O(n log n), which is well belo...
 * Range:     u8 n <= 5, u16 n <= 29, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */
#include "../utils/get_prime.h"  /* uint32_t get_prime(idx_t k); */

seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* undefined for n < 1, 0 is a safe sentinel */

    idx_t k = 0;                           /* current number of primes processed */
    seq_t sum = 0;                         /* sum of the first k primes */
    idx_t found = 0;                       /* number of primes found in the sequence */

    while (found < n) {
        ++k;
        uint32_t p = get_prime(k);         /* k‑th prime */
        sum += (seq_t)p;                   /* update sum */

        seq_t avg = sum / (seq_t)k;        /* floor average */
        if (is_prime((idx_t)avg)) {        /* check if the floor average is prime */
            ++found;
            if (found == n) return avg;
        }
    }

    return 0; /* unreachable, but keeps the compiler happy */
}
