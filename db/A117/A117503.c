/*
 * A117503  Primes among partial sums of floor(Pi*prime(k)), k=1,2,3,....
 * https://oeis.org/A117503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = the n-th prime among the partial sums of
 *            floor(π·prime(k))
 * Range:     u8 none, u16 n <= 5, u32 n <= 6, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   grow_seq_array, is_prime_big, next_prime, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation performed by generating primes, accumulating sums, and
 * collecting those sums that are prime.  Results are memoized so that
 * out‑of‑order or repeated calls incur no additional work. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/grow_seq_array.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

static const long double PI = 3.14159265358979323846264338327950288L;

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static uint64_t last_prime = 0;        /* last prime processed */
    static seq_t running_sum = 0;          /* current partial sum     */
    static seq_t *prime_sums = NULL;      /* array of prime sums     */
    static size_t ps_cap = 0;              /* capacity of prime_sums  */
    static size_t ps_len = 0;              /* number of collected sums */

    /* Initialise the array on first call */
    if (ps_cap == 0) {
        ps_cap    = 64;
        prime_sums = seq_alloc_array(ps_cap);
    }

    /* Generate primes and sums until we have at least n primes among sums */
    while (ps_len < n) {
        uint64_t p;
        if (last_prime == 0)
            p = 2;                     /* first prime */
        else
            p = next_prime(last_prime);/* next prime > last_prime */

        last_prime = p;                       /* update for next iteration */

        /* floor(π · p)  (use truncation of positive long double) */
        uint64_t add_rest = (uint64_t)((long double)p * PI);
        seq_t addend = (seq_t)add_rest;

        running_sum += addend;          /* accumulate the sum */

        /* Check if the new sum is prime */
        if (is_prime_big(running_sum)) {
            /* Grow array if necessary */
            if (ps_len >= ps_cap)
                grow_seq_array(&prime_sums, &ps_cap, ps_len + 1);
            prime_sums[ps_len++] = running_sum; /* store the prime sum */
        }
    }

    return prime_sums[n - 1];           /* 1‑based indexing as per OEIS */
}
