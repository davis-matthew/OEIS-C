/*
 * A055131  Those composite s for which A055095[s] = 2.
 * https://oeis.org/A055131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * p_n, where p_n is the (n+1)-th prime that is ≡ 1 (mod
 *            4) (i.e. the 4k+1 primes:
 *            5,13,17,29,37,41,53,61,73,89,97,101,109,113,137,149,157,173,181,193,197,229,233,241,…)
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Find the (n+1)-th 4k+1 prime. */
    idx_t count = 0;       /* number of such primes seen so far */
    idx_t candidate = 5;   /* first prime of the form 4k+1 */
    idx_t prime = 0;       /* the desired prime */

    while (true) {
        if (is_prime(candidate)) {
            if (count == n) {
                prime = candidate;
                break;
            }
            count++;
        }
        candidate += 4;    /* next number ≡ 1 (mod 4) */
    }

    /* Compute 3 * prime in the target type. */
    return (seq_t)prime * (seq_t)3;
}
