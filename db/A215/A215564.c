/*
 * A215564  C(p,k)/p for primes p and 0 < k <= p/2.
 * https://oeis.org/A215564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   ensure_capacity, gcd, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A215564: a(n) = C(p,k)/p, where the index n runs over the
 * triangular table of binomial coefficients for successive primes p,
 * with 0 < k <= p/2.  The function computes the term at position n
 * (with a(1) being the first term) by generating terms on demand.
 * It memoises all previously computed values and generates new values
 * only when requested indices are larger than those already known.
 * The implementation uses an incremental multiplicative formula that
 * avoids overflow for values that fit in the underlying `seq_t` type.
 */
#include <stddef.h>          /* for size_t */
#include "../utils/next_prime.h"
#include "../utils/gcd.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* Static state: cache of computed terms, its capacity,
     * how many terms are stored, and the last prime whose row
     * has been processed.  These are shared between calls.     */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;          /* number of terms currently stored */
    static idx_t current_prime = 2;      /* last prime whose row was generated */

    if (n == 0) return 0;               /* undefined for n < 1, return 0 */

    /* Generate rows until we have at least n terms. */
    while (cache_len < n) {
        /* Number of terms contributed by this prime p = current_prime: */
        idx_t terms = current_prime / 2;          /* floor(p/2) */

        /* Ensure the cache has enough capacity before writing. */
        size_t new_cap = cache_len + terms;
        ensure_capacity(new_cap, &cache_cap, (void **)&cache, sizeof(seq_t));

        /* Compute the sequence values for this prime. */
        seq_t val = 1;                 /* C(p,1)/p = 1 */
        idx_t k;
        for (k = 1; k <= terms; ++k) {
            cache[cache_len++] = val; /* store the value for current k */

            if (k == terms) break;    /* finished this prime row */

            /* Prepare for next k: val = val * (p-k) / (k+1)
             * Use 128-bit arithmetic when available to avoid overflow.
             * This is safe because the intermediate product fits in
             * 128 bits for any val that finally fits in seq_t.          */
            seq_t num = (current_prime - k);   /* numerator */
            seq_t den = (k + 1);               /* denominator */
#ifdef __SIZEOF_INT128__
            unsigned __int128 prod = (unsigned __int128)val * (unsigned __int128)num;
            val = (seq_t)(prod / den);
#else
            /* Fallback when 128‑bit is not available: use GCD reduction. */
            seq_t g1 = gcd(val, den);
            seq_t val_red = val / g1;
            seq_t den_red = den / g1;
            seq_t g2 = gcd(num, den_red);
            seq_t num_red = num / g2;
            den_red = den_red / g2;
            /* After reductions, den_red should be 1; otherwise use 128‑bit. */
            if (den_red == 1) {
                val = val_red * num_red;
            } else {
                unsigned __int128 prod = (unsigned __int128)val_red * (unsigned __int128)num_red;
                val = (seq_t)(prod / den_red);
            }
#endif
        }

        /* Move to the next prime. */
        current_prime = (idx_t)next_prime((uint64_t)current_prime);
    }

    return cache[n - 1];   /* cache is 0‑based, n is 1‑based */
}
