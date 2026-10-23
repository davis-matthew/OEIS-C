/*
 * A171262  Numbers k such that phi(k) = 2*phi(k+1).
 * https://oeis.org/A171262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 498, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching for k such that φ(k) = 2 φ(k+1) using trial
 * division for φ.  The function caches found terms for subsequent
 * requests. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Runtime cache of sequence terms. */
static seq_t *seq_cache = NULL;
static idx_t seq_len   = 0;   /* number of cached elements      */
static idx_t seq_cap   = 0;   /* allocated capacity             */
static idx_t checked_up_to = 0; /* largest k examined so far     */

/* Utility: ensure the cache can hold at least 'need' elements. */
static void grow_seq_cache(idx_t need)
{
    if (seq_cap >= need) return;
    idx_t new_cap = seq_cap ? seq_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Append a new term to the cache. */
static void push_seq(seq_t val)
{
    grow_seq_cache(seq_len + 1);
    seq_cache[seq_len++] = val;
}

/* φ(n) for 64‑bit n via trial division. */
static uint64_t phi64(uint64_t n)
{
    if (n <= 1) return n;
    uint64_t result = n;
    uint64_t temp = n;

    if (temp % 2 == 0) {
        result -= result / 2;
        while (temp % 2 == 0) temp /= 2;
    }

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            result -= result / p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) result -= result / temp;
    return result;
}

/* Extend the cache until at least 'want' terms are present. */
static void compute_until(idx_t want)
{
    if (seq_len >= want) return;

    for (uint64_t k = checked_up_to + 1;; ++k) {
        uint64_t phi_k  = phi64(k);
        uint64_t phi_k1 = phi64(k + 1);

        /* φ(k) == 2 φ(k+1)  <=>  φ(k) even and φ(k)/2 == φ(k+1)   */
        if ((phi_k & 1ULL) == 0 && phi_k / 2 == phi_k1) {
            push_seq((seq_t)k);
            if (seq_len >= want) {
                checked_up_to = k;
                break;
            }
        }
    }
    /* In case the loop terminates without satisfying the break
     * (theoretically impossible for finite n). */
    checked_up_to++;
}

/* Public interface required by the OEIS test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    compute_until(n);              /* ensure we have at least n terms */
    return seq_cache[n - 1];       /* OEIS uses 1‑based indexing   */
}
