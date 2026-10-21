/*
 * A095281  Upper Wythoff primes, i.e., primes in A001950.
 * https://oeis.org/A095281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime in the upper Wythoff sequence * b_k =
 *            floor(k*φ^2) = floor(k*(φ+1)) = floor(k*φ)+k. * The function
 *            generates successive b_k values, tests each for * primality
 *            (using is_prime_big), and caches the primes found. * a(1) is the
 *            first such prime
 * Range:     u8 n <= 22, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset 1: a(1) is defined */

    /* cached primes */
    static seq_t *primes = NULL;      /* array of primes */
    static size_t primes_size = 0;    /* number of stored primes */
    static size_t primes_cap  = 0;    /* current allocation capacity */

    /* generation state */
    static idx_t next_k = 1;          /* next k to generate b_k */

    /* constants for φ */
    static const long double sqrt5 = sqrtl(5.0L);
    static const long double phi   = (1.0L + sqrt5) / 2.0L; /* φ */

    /* generate primes until we have at least n of them */
    while (primes_size < n) {
        /* floor(k*φ) computed in long double and cast to idx_t */
        long double a_ld = (long double)next_k * phi + 1e-12L;
        idx_t a_k = (idx_t)a_ld;                /* floor(k*φ) */
        idx_t b_k_uint = a_k + next_k;          /* b_k = floor(k*φ)+k */

        seq_t val = (seq_t)b_k_uint;            /* promote to seq_t */

        if (is_prime_big(val)) {
            grow_seq(&primes, &primes_cap, primes_size + 1);
            primes[primes_size++] = val;
        }

        ++next_k;
    }

    return primes[n - 1];
}
