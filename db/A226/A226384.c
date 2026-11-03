/*
 * A226384  Numbers k such that rad(phi(k)) = phi(rad(k)).
 * https://oeis.org/A226384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 5419, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9524 (2026-11-03)
 * Helpers:   mem, radical, totient
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
#include <stdlib.h>
#include "../utils/radical.h"
#include "../utils/totient.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

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
        seq_t phi_k   = totient(max_checked);             /* φ(k)   */
        seq_t rad_phi = radical((idx_t)phi_k);            /* rad(φ(k)) */

        /* Compute rad(k) and φ(rad(k)) */
        seq_t rad_k    = radical(max_checked);            /* rad(k)   */
        seq_t phi_rad = totient((idx_t)rad_k);            /* φ(rad(k)) */

        /* If the equality holds, store the value */
        if (rad_phi == phi_rad) {
            append_candidate(&cache, &cache_len, &cache_cap,
                             (seq_t)max_checked);
        }
    }

    /* Return the requested term (1‑based) */
    return cache[(size_t)n - 1];
}
