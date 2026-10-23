/*
 * A138969  Positions of the primes congruent to 2 mod 3 when all primes
 *          except 3 are listed in order.
 * https://oeis.org/A138969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138969: positions of primes ≡2 mod 3 among primes ≠3,
   computed by incremental prime search. */
#include <stdbool.h>

static bool is_prime_simple(idx_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    for (idx_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for offset 1 */

    idx_t pos = 0;   /* position among primes ≠ 3 */
    idx_t count = 0; /* number of primes ≡2 mod 3 found */

    for (idx_t cand = 2; ; ++cand) {
        if (cand == 3) continue;                /* 3 is excluded */
        if (cand > 2 && (cand & 1) == 0) continue; /* skip even numbers */

        if (!is_prime_simple(cand)) continue;    /* not prime */

        ++pos;                                 /* we have a prime in the list */
        if ((cand % 3) == 2) {                  /* prime ≡2 mod 3? */
            ++count;
            if (count == n) return (seq_t)pos;
        }
    }
}
