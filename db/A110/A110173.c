/*
 * A110173  Least k such that phi(n) = phi(k) + phi(n-k) for 0<k<n, or 0 if
 *          there is no such k, where phi is Euler's totient function.
 * https://oeis.org/A110173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 613, u16 n <= 9434, u32 n <= 9434, u64 n <= 9434,
 *            u128 n <= 9434, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = least k such that 0<k<n and phi(n)=phi(k)+phi(n-k);
   returns 0 if no such k.  Uses a cached Euler totient sieve. */

#include <stdlib.h>

/* Cached totient values; indexed 1..N. */
static seq_t *phi_cache = NULL;
static idx_t phi_cache_size = 0;

/* Ensure phi_cache contains values up to n (inclusive). */
static void ensure_phi(idx_t n) {
    if (n <= phi_cache_size)
        return;

    /* Allocate space for 0..n (we ignore index 0). */
    seq_t *new_cache = (seq_t *)realloc(phi_cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – abort (should not happen in normal use). */
        abort();
    }

    /* Initialize all entries to their index value. */
    for (idx_t i = 1; i <= n; ++i)
        new_cache[i] = (seq_t)i;

    /* Euler Sieve to compute phi values. */
    for (idx_t p = 2; p <= n; ++p) {
        if (new_cache[p] == (seq_t)p) {          /* p is prime */
            for (idx_t m = p; m <= n; m += p) {
                new_cache[m] -= new_cache[m] / p;
            }
        }
    }

    phi_cache = new_cache;
    phi_cache_size = n;
}

/* Retrieve phi(n) from the cache. */
static seq_t get_phi(idx_t n) {
    if (n == 0) return 0;
    ensure_phi(n);
    return phi_cache[n];
}

seq_t a(idx_t n)
{
    if (n < 3)                       /* no admissible k for n = 1 or 2 */
        return (seq_t)0;

    seq_t phi_n = get_phi(n);

    for (idx_t k = 1; k < n; ++k) {
        if (get_phi(k) + get_phi(n - k) == phi_n)
            return (seq_t)k;
    }
    return (seq_t)0;
}
