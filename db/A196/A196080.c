/*
 * A196080  Numerators of the sum of the n-th partial sums of the expansions
 *          of e and 1/e.
 * https://oeis.org/A196080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the sum_{k=0}^{⌊n/2⌋} 2/(2k)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define PRIME_MAX 20000

static int primes[PRIME_MAX + 1];
static int prime_count = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized)
        return;

    for (int i = 2; i <= PRIME_MAX; ++i)
        primes[i] = 1;

    for (int i = 2; i * i <= PRIME_MAX; ++i) {
        if (primes[i]) {
            for (int j = i * i; j <= PRIME_MAX; j += i)
                primes[j] = 0;
        }
    }

    for (int i = 2; i <= PRIME_MAX; ++i)
        if (primes[i])
            primes[prime_count++] = i;

    primes_initialized = true;
}

seq_t a(idx_t n)
{
    /* floor division by 2 */
    idx_t m = n >> 1;

    /* compute U_m = (2m)(2m-1)U_{m-1} + 2, U_0 = 2 */
    seq_t U = (seq_t)2;
    for (idx_t k = 1; k <= m; ++k) {
        seq_t A = (seq_t)(2 * k) * (seq_t)(2 * k - 1);
        U = A * U + (seq_t)2;
    }

    seq_t u = U;          /* copy for reduction */

    init_primes();        /* ensure prime list is ready */

    idx_t limit = 2 * m;
    for (int pi = 0; pi < prime_count && primes[pi] <= limit; ++pi) {
        idx_t p = primes[pi];

        /* exponent of p in (2m)! using Legendre's formula */
        idx_t exp_fact = 0;
        idx_t tmp = limit;
        while (tmp) {
            tmp /= p;
            exp_fact += tmp;
        }

        seq_t prime_seq = (seq_t)p;
        idx_t times = 0;
        while (times < exp_fact && (u % prime_seq) == 0) {
            u /= prime_seq;
            ++times;
        }
    }

    return u;   /* reduced numerator */
}
