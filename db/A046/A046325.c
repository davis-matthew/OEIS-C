/*
 * A046325  Product of 7 successive primes.
 * https://oeis.org/A046325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of primes p_n · p_{n+1} · … · p_{n+6}
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 100, u128 n <= 1000,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *primes = NULL;     /* array of the first primes seen so far */
static size_t primes_cnt = 0;       /* number of primes stored */

/* Ensure that at least `need` primes are available in the global array. */
static void
ensure_primes(size_t need)
{
    if (primes_cnt >= need)
        return;

    uint32_t *tmp;
    size_t tmp_cnt;
    /* sieve_first_n_primes allocates and returns an array of the first `need`
       primes. */
    sieve_first_n_primes(need, &tmp, &tmp_cnt);

    if (primes != NULL)
        free(primes);
    primes = tmp;
    primes_cnt = tmp_cnt;
}

/* Return the n-th element of OEIS A046325.  An offset of 1 is used
   (so a(1) is the product of the first 7 primes).  The function
   works for any n ≥ 1, independent of the order of calls. */
seq_t
a(idx_t n)
{
    /* We need the primes p_n through p_{n+6}.  Since the array is
       zero‑based, the indices are n-1 … n+5. */
    size_t need = (size_t)n + 6;               /* at least up to prime n+6 */
    ensure_primes(need);

    seq_t prod = 1;
    /* multiply the 7 consecutive primes */
    for (size_t i = (size_t)n - 1; i < (size_t)n + 6; ++i) {
        prod *= (seq_t)primes[i];
    }
    return prod;
}
