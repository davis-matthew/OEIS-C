/*
 * A226384  Numbers k such that rad(phi(k)) = phi(rad(k)).
 * https://oeis.org/A226384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 5419, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, phi, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A226384: numbers k such that rad(φ(k)) = φ(rad(k)) */
/* Computation: iteratively test successive integers k, using the helper
   functions rad() and phi() (both return seq_t), and store those that
   satisfy the equality. The sequence is generated on demand and cached,
   so arbitrary order of calls is supported. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/rad.h"
#include "../utils/phi.h"
#include "../utils/append_candidate.h"

/* Static cache for the generated terms */
static seq_t *cache   = NULL;  /* array of results */
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity of cache */

/* Highest integer examined so far */
static idx_t max_checked = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for the offset 1, but harmless */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* Examine next integer */
        ++max_checked;

        /* Compute φ(k) and rad(φ(k)) */
        seq_t phi_k   = phi(max_checked);             /* φ(k)   */
        seq_t rad_phi = rad((idx_t)phi_k);            /* rad(φ(k)) */

        /* Compute rad(k) and φ(rad(k)) */
        seq_t rad_k    = rad(max_checked);            /* rad(k)   */
        seq_t phi_rad = phi((idx_t)rad_k);            /* φ(rad(k)) */

        /* If the equality holds, store the value */
        if (rad_phi == phi_rad) {
            append_candidate(&cache, &cache_len, &cache_cap,
                             (seq_t)max_checked);
        }
    }

    /* Return the requested term (1‑based) */
    return cache[(size_t)n - 1];
}
