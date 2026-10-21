/*
 * A055772  Square root of largest square dividing n!.
 * https://oeis.org/A055772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt( largest square dividing n! ) (i.e. ∏ p^⌊v_p(n!) /
 *            2⌋ over all primes p ≤ n )
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 24, u64 n <= 41, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard C headers for allocation and booleans */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1; return 0 for n < 1. */
    if (n < 1) return (seq_t)0;

    /* For n = 1 the factorial is 1, so the answer is 1. */
    if (n < 2) return (seq_t)1;

    /* Generate the list of primes up to n using a sieve. */
    size_t size = (size_t)n + 1;
    bool *is_prime = (bool *)calloc(size, sizeof(bool));
    if (!is_prime) return (seq_t)0;                /* allocation failure */
    for (size_t i = 2; i < size; ++i) is_prime[i] = true;

    for (size_t i = 2; i * i <= (size_t)n; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j < size; j += i)
                is_prime[j] = false;
        }
    }

    seq_t result = (seq_t)1;

    /* For every prime p ≤ n, compute v_p(n!) and multiply by p^(floor(v_p/2)). */
    for (size_t p = 2; p < size; ++p) {
        if (!is_prime[p]) continue;

        /* v_p(n!) = ⌊n/p⌋ + ⌊n/p²⌋ + …   */
        idx_t exp = 0;
        idx_t m = n;
        while (m) {
            m /= (idx_t)p;
            exp += m;
        }
        exp >>= 1;             /* floor(exp/2) */

        if (exp == 0) continue;

        seq_t power = (seq_t)1;
        for (idx_t k = 0; k < exp; ++k)
            power *= (seq_t)p;   /* exponentiation by repeated multiplication */

        result *= power;
    }

    free(is_prime);
    return result;
}
