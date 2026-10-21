/*
 * A070546  a(n) = Card( k, 0<k<=n such that k is relatively prime to
 *          tau(k)=A000005(k)).
 * https://oeis.org/A070546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 551, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A070546: a(n) = Card{ k | 1 ≤ k ≤ n and gcd(k, τ(k)) = 1 }.
   Here τ(k) is the number of positive divisors of k. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* no valid k for n < 1 */

    size_t N = (size_t)n;               /* size for array allocation */

    /* Allocate array for τ(k) for k from 0 to N. */
    seq_t *tau = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!tau) return 0;                  /* allocation failed – return 0 */

    /* Initialize to zero and compute divisor counts via a simple sieve. */
    for (size_t i = 0; i <= N; ++i) tau[i] = 0;
    for (size_t i = 1; i <= N; ++i) {
        for (size_t j = i; j <= N; j += i) {
            tau[j] += 1;
        }
    }

    /* Count numbers k ≤ N that are coprime to τ(k). */
    seq_t count = 0;
    for (size_t k = 1; k <= N; ++k) {
        seq_t a_val = (seq_t)k;
        seq_t b_val = tau[k];

        /* Euclidean algorithm for gcd.  Works for any unsigned integer type. */
        seq_t x = a_val, y = b_val;
        while (y != 0) {
            seq_t r = x % y;
            x = y;
            y = r;
        }

        if (x == 1) count++;            /* gcd(k, τ(k)) == 1 */
    }

    free(tau);
    return count;
}
