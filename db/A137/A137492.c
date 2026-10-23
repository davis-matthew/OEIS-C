/*
 * A137492  Numbers with 29 divisors.
 * https://oeis.org/A137492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000040(n)^(28) (29‑divisor numbers: 28th power of the
 *            n‑th prime)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/sieve_first_n_primes.h"

/* Returns the n‑th number with exactly 29 divisors.  For n=1 the result is
   2^28.  The function builds the list of primes only when required and is
   memoised so calls with arbitrary n are fast. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n<1, return 0 */
    static uint32_t *primes = NULL;           /* array of primes (u32 fine) */
    static size_t primes_cnt = 0;             /* number of primes cached   */

    /* Need more primes? Re‑build cache for the requested index. */
    if ((size_t)n > primes_cnt) {
        free(primes);                         /* free old array if any     */
        sieve_first_n_primes(n, &primes, &primes_cnt);
    }

    seq_t base = (seq_t)primes[n - 1];         /* n‑th prime (offset 1) */
    return big_pow(base, (seq_t)28);          /* 28th power of the prime   */
}
