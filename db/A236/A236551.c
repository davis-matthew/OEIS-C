/*
 * A236551  Primes formed from concatenation of PrimePi(n) and prime(n).
 * https://oeis.org/A236551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 41, u64 n <= 1461,
 *            u128 n <= 1461, big any n
 * Helpers:   compute_pi, concat_parts, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes formed by concatenation of Pi(n) and prime(n) */
/* a(n) = nth smallest prime of the form concat(Pi(k), prime(k)). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/get_prime.h"
#include "../utils/concat_parts.h"
#include "../utils/compute_pi.h"

/* Dynamic cache of sequence values */
static seq_t  *seq_cache = NULL;
static size_t  seq_len   = 0;
static size_t  seq_cap   = 0;
static idx_t   last_k    = 0;

seq_t a(idx_t n) {
    /* n is 1-based index in the OEIS sequence */
    if (n == 0) return (seq_t)0;  /* undefined, but guard against underflow */

    /* Generate sequence values until we have at least n terms */
    while (seq_len < (size_t)n) {
        idx_t k = ++last_k;                  /* next k to test */
        idx_t pi_k = (idx_t)compute_pi(k);   /* number of primes <= k   */
        uint32_t pk = get_prime(k);           /* kth prime              */
        seq_t  candidate = concat_parts(pi_k, (idx_t)pk);

        if (is_prime_big(candidate)) {
            /* Ensure cache capacity */
            if (seq_len == seq_cap) {
                seq_cap = (seq_cap > 0) ? seq_cap << 1 : 16;
                seq_cache = (seq_t *)realloc(seq_cache,
                                             seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = candidate;
        }
    }
    return seq_cache[n - 1];
}
