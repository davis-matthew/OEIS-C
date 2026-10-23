/*
 * A127335  Numbers that are the sum of 8 successive primes.
 * https://oeis.org/A127335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1024, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

/* Calculates a(n) = sum of 8 consecutive primes starting from prime(n).
 * The primes are generated with a sieve and cached between calls. */

static uint32_t *primes_arr = NULL;   /* array of primes, 0‑based */
static idx_t  primes_cached = 0;      /* number of primes stored */

/* Ensure that primes_arr contains at least (need_index+1) primes. */
static void
ensure_primes(idx_t need_index)
{
    /* need_index is 0‑based index of the last required prime. */
    if (primes_cached > need_index)
        return;
    idx_t count_needed = need_index + 1;
    uint32_t *tmp;
    size_t   cnt;
    sieve_first_n_primes(count_needed, &tmp, &cnt); /* cnt == count_needed */
    if (primes_arr != NULL)
        free(primes_arr);
    primes_arr  = tmp;
    primes_cached = cnt;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;                    /* behaviour for n < 1 is undefined */
    idx_t last_index = n + 6;         /* 0‑based index of the last prime */
    ensure_primes(last_index);
    seq_t sum = 0;
    for (idx_t i = n - 1; i <= last_index; ++i)
        sum += (seq_t)primes_arr[i];
    return sum;
}
