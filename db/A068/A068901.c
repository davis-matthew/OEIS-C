/*
 * A068901  Least number that when added to the n-th prime gives a multiple of
 *          n.
 * https://oeis.org/A068901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, look
 * Formula:   a(n) = Min_{k≥0 | prime(n)+k ≡ 0 (mod n)} = (n - prime(n) mod n)
 *            mod n
 * Range:     u8 n <= 442, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses nth prime retrieval via _prime_limit_for_idx and build_primes_upto */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"   /* _prime_limit_for_idx */
#include "../utils/build_primes_upto.h"     /* build_primes_upto */

static uint64_t *prime_arr   = NULL;   /* array of primes */
static size_t    prime_cnt   = 0;      /* number of primes in prime_arr */
static idx_t     prime_limit = 0;      /* largest limit used for prime generation */

static void init_primes(idx_t n)
{
    /* Determine limit needed to obtain at least n primes */
    idx_t needed_limit = _prime_limit_for_idx(n);
    if (prime_arr == NULL || needed_limit > prime_limit) {
        /* re-generate primes with larger limit */
        if (prime_arr != NULL) free(prime_arr);
        build_primes_upto(needed_limit, &prime_arr, &prime_cnt);
        prime_limit = needed_limit;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but return 0 for safety */

    init_primes(n);

    /* prime(n) is the (n-1)-th element of prime_arr (0‑based index) */
    uint64_t p = prime_arr[n - 1];

    /* Compute remainder of prime modulo n */
    seq_t r = (seq_t)(p % n);

    /* a(n) = (n - r) mod n */
    seq_t k = (seq_t)((seq_t)n - r);
    if (k == (seq_t)n) k = 0;

    return k;
}
