/*
 * A139789  Numbers of primes < n-th-prime such that prime(n) + 4 is prime.
 * https://oeis.org/A139789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   build_primes_upto, get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = number of primes p < p_n such that p + 4 is prime.
   The function uses a sieve to generate the primes up to the required
   limit and keeps a prefix sum of the “p + 4 is prime” flags.  */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"
#include "../utils/build_primes_upto.h"

/* Persistent state for the sieve and prefix sums. */
static uint64_t *primes      = NULL;   /* array of primes up to current limit */
static size_t   primes_cnt  = 0;      /* number of primes stored            */
static bool     *flags      = NULL;   /* flags for “prime + 4 is prime”     */
static seq_t    *prefix     = NULL;   /* prefix sums: prefix[i] = count of
                                        flags among first i primes           */

/* Rebuild the sieve and prefix sums for the first n primes. */
static void rebuild_up_to(idx_t n)
{
    /* Get the nth prime and build all primes up to that value. */
    idx_t limit = get_prime(n);                 /* nth prime value   */
    uint64_t *new_primes = NULL;                /* new primes array  */
    size_t new_cnt = 0;                          /* number of primes  */

    build_primes_upto(limit, &new_primes, &new_cnt);

    /* Free any previous data. */
    free(primes);
    free(flags);
    free(prefix);

    /* Adopt the newly computed data. */
    primes      = new_primes;
    primes_cnt  = new_cnt;
    flags       = (bool *)malloc(new_cnt * sizeof(bool));
    prefix      = (seq_t *)malloc((new_cnt + 1) * sizeof(seq_t));

    /* Compute flags and prefix sums. */
    prefix[0] = (seq_t)0;
    for (size_t i = 0; i < new_cnt; ++i) {
        bool f = is_prime_small((seq_t)primes[i] + (seq_t)4);
        flags[i] = f;
        prefix[i + 1] = prefix[i] + (f ? (seq_t)1 : (seq_t)0);
    }
}

/* Make sure we have at least the first n primes and their flags. */
static void ensure_primes(idx_t n)
{
    if (n <= primes_cnt) return;   /* already sufficient */
    rebuild_up_to(n);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined, but keep safe */
        return (seq_t)0;

    ensure_primes(n);                /* ensure data for first n primes */

    /* Count of primes p < p_n with p + 4 prime   ===> prefix[n-1]   */
    return prefix[n - 1];
}
