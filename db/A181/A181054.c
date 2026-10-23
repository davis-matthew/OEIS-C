/*
 * A181054  Numbers n such that Sum_{k=1..n} (-1)^(n-k)*sigma(k) is prime.
 * https://oeis.org/A181054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 25, u32 n <= 31, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   append_candidate, is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A181054: an is the index n such that the alternating sum
 *   S(n) = Σ_{k=1..n} (-1)^(n-k) * σ(k)
 *   is a prime number.  This function returns the nth such index.  It
 *   builds the list on demand and works for any implementation of
 *   seq_t and idx_t that the build system provides.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* Cache of found indices */
    static seq_t *indices  = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    /* State for the alternating sum S(m) = Σ_{k=1..m} (-1)^(m-k)*σ(k) */
    static idx_t m = 0;          /* current maximal m examined */
    static int64_t S = 0;        /* signed sum, may be negative */

    /* Grow the cache as needed */
    while (len < n) {
        ++m; /* examine the next integer */

        seq_t sigma_val = sigma_of(m);
        int64_t sigma = (int64_t)sigma_val; /* σ(m) fits in int64_t for m ≤ 10 000 */

        /* S(m) = -S(m-1) + σ(m) */
        S = -S + sigma;

        /* Only positive sums can be prime */
        if (S > 1) {
            seq_t s = (seq_t)S; /* cast back to seq_t for primality test */
            if (is_prime_big(s))
                append_candidate(&indices, &len, &cap, (seq_t)m);
        }
    }

    return indices[n - 1];
}
