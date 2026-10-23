/*
 * A106634  Number of ways to express n as i*j+k*l, with i,j,k,l in the range
 *          [0..n].
 * https://oeis.org/A106634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+2)*tau(n) + Sum_{i=1}^{n-1} tau(i)*tau(n-i), where
 *            tau(m) is the number of positive divisors of m. a(0) = 1
 * Range:     u8 n <= 11, u16 n <= 479, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *tau = NULL;
static idx_t tau_max = 0;

static void ensure_tau(idx_t n)
{
    if (n <= tau_max) return;

    /* Allocate a fresh array up to n (size n+1 to include 0). */
    seq_t *new_tau = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!new_tau) return;  /* allocation failed – leave uninitialized */

    memset(new_tau, 0, (n + 1) * sizeof(seq_t));

    /* Sieve-like computation of divisor counts. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            new_tau[j] += (seq_t)1;
        }
    }

    free(tau);
    tau = new_tau;
    tau_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    ensure_tau(n);
    seq_t res = (seq_t)(4*n + 2) * tau[n];

    for (idx_t i = 1; i < n; ++i) {
        res += tau[i] * tau[n - i];
    }
    return res;
}
