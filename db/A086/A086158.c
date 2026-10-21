/*
 * A086158  Concatenation of the smaller members of first n pairs of twin
 *          primes.
 * https://oeis.org/A086158
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the smaller members of the first n‑1
 *            twin prime pairs (offset 2). The sequence is built incrementally
 *            and memoised for repeated calls
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                       /* offset is 2 */

    const idx_t pairs_needed = n - 1;    /* number of twin pairs required */

    /* Static state for incremental construction */
    static bool          built           = false;
    static seq_t        *concat_vec      = NULL;
    static size_t        concat_cap      = 0;
    static size_t        concat_len      = 0;
    static bool          *is_prime       = NULL;
    static idx_t          limit           = 0;

    if (!built) {
        /* -----------------------------------------------------------------
           Build a sieve up to a fixed limit that guarantees at least 10 000
           twin prime pairs.  2 000 000 yields about 12 190 twin pairs, which
           is more than enough for all required n.  ------------------------------------------------- */
        limit = 2000000;
        size_t sz = (size_t)limit + 1;

        is_prime = (bool *)malloc(sz);
        if (is_prime == NULL)
            return 0;                       /* allocation failure */

        for (size_t i = 0; i < sz; ++i)
            is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;

        for (idx_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_prime[j] = false;
            }
        }

        /* -----------------------------------------------------------------
           Build the sequence of concatenated numbers for twin prime pairs.
           ----------------------------------------------------------------- */
        concat_cap = 20000;                /* initial capacity */
        concat_vec = (seq_t *)malloc(concat_cap * sizeof(seq_t));
        if (concat_vec == NULL) {
            free(is_prime);
            return 0;
        }
        concat_len = 0;

        seq_t current = 0;
        bool  first   = true;

        for (idx_t i = 2; i + 2 <= limit; ++i) {
            if (!is_prime[i] || !is_prime[i + 2])
                continue;                   /* not a twin pair */

            /* number of decimal digits of the smaller prime i */
            idx_t dig = 0, t = i;
            do {
                ++dig;
                t /= 10;
            } while (t != 0);

            /* extend storage if necessary */
            if (concat_len == concat_cap) {
                concat_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(concat_vec,
                                              concat_cap * sizeof(seq_t));
                if (tmp == NULL) {
                    free(is_prime);
                    free(concat_vec);
                    return 0;
                }
                concat_vec = tmp;
            }

            if (first) {
                current = (seq_t)i;
                first   = false;
            } else {
                /* multiply by 10^dig and add the new i */
                seq_t mult = 1;
                for (idx_t k = 0; k < dig; ++k)
                    mult *= (seq_t)10;
                current = current * mult + (seq_t)i;
            }

            concat_vec[concat_len++] = current;
        }

        built = true;
    }

    /* --------------------------------------------------------------------
       Return the requested term.
       -------------------------------------------------------------------- */
    if (pairs_needed > concat_len)
        return 0;                       /* not enough precomputed pairs */

    return concat_vec[pairs_needed - 1];
}
