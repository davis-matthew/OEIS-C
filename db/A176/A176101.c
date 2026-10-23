/*
 * A176101  a(n) = 4^(composite(n) - 4) mod composite(n+4).
 * https://oeis.org/A176101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 4^(composite(n) - 4) mod composite(n+4) (OEIS A176101)
 * Range:     u8 n <= 197, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

/* storage for the list of composite numbers (1‑based indexing) */
static idx_t *composites = NULL;      /* array of composite values     */
static idx_t composites_capacity = 0; /* allocated size                 */
static idx_t composites_size = 0;      /* number of composites stored   */

/* Ensure that composites array contains at least `count` entries
   (i.e. up to composite(count)).  A bitwise expansion strategy
   is used for efficiency. */
static void ensure_composites(idx_t count)
{
    if (composites_size >= count)
        return;

    /* Grow the array if needed. */
    if (composites_capacity < count) {
        size_t new_cap = composites_capacity == 0 ? 64 : composites_capacity;
        while (new_cap < count)
            new_cap <<= 1;                  /* double until big enough   */
        composites = (idx_t *)realloc(composites, new_cap * sizeof(idx_t));
        composites_capacity = new_cap;
        if (composites == NULL)            /* allocation failed – abort   */
            return;                        /* (caller will get undefined) */
    }

    /* Fill the array with composite numbers until we reach `count`. */
    idx_t candidate = composites_size == 0 ? 4 : composites[composites_size - 1] + 1;
    while (composites_size < count) {
        if (!is_prime(candidate))
            composites[composites_size++] = candidate;
        ++candidate;
    }
}

/* Fast modular exponentiation: base^exp mod mod. */
static seq_t mod_pow_seq(seq_t base, uint64_t exp, seq_t mod)
{
    if (mod == 0)                     /* safety: should never happen   */
        return 0;
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = (seq_t)(((__uint128_t)result * b) % mod);
        b = (seq_t)(((__uint128_t)b * b) % mod);
        exp >>= 1ULL;
    }
    return result;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* We need composite(n) and composite(n+4). */
    ensure_composites(n + 4);          /* ensure enough composites stored */

    idx_t comp_n   = composites[n - 1];   /* composite(n)   */
    idx_t comp_np4 = composites[n + 3];   /* composite(n+4) */

    /* Exponent is composite(n) - 4.  For n=1 this is 0. */
    uint64_t exp = (uint64_t)(comp_n - 4);

    /* Compute 4^exp mod composite(n+4). */
    return mod_pow_seq((seq_t)4, exp, (seq_t)comp_np4);
}
