/*
 * A236225  Sum of the seventeenth powers of the first n primes.
 * https://oeis.org/A236225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} prime(k)^17
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 6, u128 n <= 40,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static uint64_t *primes = NULL;          /* primes[0] is prime(1) */
    static size_t primes_cnt = 0;            /* number of primes stored */

    static seq_t *cum = NULL;                /* cumulative sums, cum[0] = 0 */
    static size_t cum_size = 0;              /* allocated capacity for cum */

    static idx_t current_n = 0;              /* highest n summed so far */

    /* ------------------------------------------------------------------
     * Ensure we have at least n primes.  Re‑build the list if necessary.
     * ------------------------------------------------------------------ */
    if ((size_t)n > primes_cnt) {
        /* free previous primes array if present */
        if (primes) {
            free(primes);
            primes = NULL;
            primes_cnt = 0;
        }

        /* initial guess: about 20×n is enough for n≤10000 */
        idx_t limit = (n > 5) ? n * (idx_t)20 : (idx_t)100;
        for (;;) {
            uint64_t *tmp = NULL;
            size_t cnt = 0;
            build_primes_upto(limit, &tmp, &cnt);
            if (cnt >= (size_t)n) {
                primes = tmp;
                primes_cnt = cnt;
                break;
            }
            free(tmp);
            limit <<= 1;   /* double the limit and try again */
        }
    }

    /* ------------------------------------------------------------------
     * Allocate or grow the cumulative sum array.
     * ------------------------------------------------------------------ */
    if (cum_size < (size_t)(n + 1)) {
        size_t new_size = cum_size ? cum_size * 2 : 4;
        if (new_size < (size_t)(n + 1))
            new_size = n + 1;

        /* allocate new array */
        seq_t *new_cum = NULL;
#if defined(__cplusplus)
        new_cum = new seq_t[new_size];
#else
        new_cum = (seq_t *)malloc(new_size * sizeof(seq_t));
#endif
        if (!new_cum)
            return (seq_t)0; /* out of memory – caller must handle */

        /* copy existing data */
        for (size_t i = 0; i < cum_size; ++i)
            new_cum[i] = cum[i];

        /* initialise cum[0] if this is the first allocation */
        if (cum_size == 0)
            new_cum[0] = (seq_t)0;

        /* free old array */
        if (cum) {
#if defined(__cplusplus)
            delete[] cum;
#else
            free(cum);
#endif
        }

        cum = new_cum;
        cum_size = new_size;
    }

    /* ------------------------------------------------------------------
     * Compute new sums up to n if necessary.
     * ------------------------------------------------------------------ */
    for (idx_t i = current_n + 1; i <= n; ++i) {
        seq_t p   = (seq_t)primes[(size_t)i - 1]; /* prime(i) */
        seq_t pow = big_pow(p, (seq_t)17);         /* prime(i)^17 */
        cum[(size_t)i] = cum[(size_t)i - 1] + pow;
    }
    current_n = n;

    return cum[(size_t)n];
}
