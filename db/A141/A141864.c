/*
 * A141864  Frequency of A038511 in the interval [(n-1)*3*5*7,n*3*5*7].
 * https://oeis.org/A141864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A141864: a(n) = number of composite m ∈ [(n-1)·105, n·105] whose prime
    factors are all ≥ 11.  The sole artificial term m = 1 is counted in
    a(1).  This implementation builds a prefix table of qualifying
    numbers up to the largest required bound and uses it to answer each
    query in O(1).  The sieve is performed only once and grows only as
    needed.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* external integer types supplied by the build */
extern seq_t a(idx_t);

static idx_t  cur_max_limit = 0;   /* largest bound that has been precomputed */
static seq_t *prefix        = NULL;/* prefix[i] = number of qualifying m ≤ i */

static void init_up_to(idx_t limit)
{
    if (limit <= cur_max_limit) return;          /* already computed */

    /* free previous prefix storage */
    if (prefix != NULL) {
        free(prefix);
        prefix = NULL;
    }

    size_t sz = (size_t)(limit + 1);              /* array size */

    /* 1. Sieve of Eratosthenes for primality */
    uint8_t *is_prime = (uint8_t*)malloc(sz);
    if (is_prime == NULL) return;                /* allocation failure */
    for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
    is_prime[0] = 0; is_prime[1] = 0;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p) {
                is_prime[j] = 0;
            }
        }
    }

    /* 2. Mark numbers divisible by any of 2,3,5,7 */
    uint8_t *div_small = (uint8_t*)malloc(sz);
    if (div_small == NULL) { free(is_prime); return; }
    for (size_t i = 0; i < sz; ++i) div_small[i] = 0;
    const idx_t small_primes[4] = {2,3,5,7};
    for (int k = 0; k < 4; ++k) {
        idx_t p = small_primes[k];
        for (idx_t i = p; i <= limit; i += p) {
            div_small[i] = 1;
        }
    }

    /* 3. Build prefix of qualifying composite numbers. */
    prefix = (seq_t*)malloc(sz * sizeof(seq_t));
    if (prefix == NULL) {
        free(is_prime);
        free(div_small);
        return;
    }
    prefix[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t val = 0;
        if (i == 1) {
            val = 1;                       /* artificial term 1 */
        } else if (!is_prime[i] && !div_small[i]) {
            val = 1;                       /* composite, no small prime factor */
        }
        prefix[i] = prefix[i-1] + val;
    }

    cur_max_limit = limit;                    /* record new maximum */

    /* free temporary arrays to conserve memory */
    free(is_prime);
    free(div_small);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, but safe */
    if (n == 1) return 1;                     /* artificial first term */

    idx_t hi = n * 105ull;                    /* upper bound of interval */
    if (hi > cur_max_limit) init_up_to(hi);   /* grow table if necessary */

    idx_t lo = (n-1) * 105ull;                /* lower bound of interval */

    /* prefix[lo-1] is safe because lo >= 105 => lo-1 >= 104 */
    return prefix[hi] - prefix[lo-1];
}
