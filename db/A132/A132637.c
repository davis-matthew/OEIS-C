/*
 * A132637  Composite number C(n) raised to power C(n).
 * https://oeis.org/A132637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002808(n)^A002808(n) (nth composite number to its own
 *            power)
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   ipow, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Static cache for composite numbers */
static seq_t *composites = NULL;
static size_t composites_len = 0;   /* number of composites stored */
static size_t composites_cap = 0;   /* allocated capacity */
static idx_t next_candidate = 4;    /* next integer to test for composite */

/* Ensure that at least n composite numbers are precomputed */
static void ensure_composites(idx_t n)
{
    while (composites_len < (size_t)n) {
        /* Find the next composite integer */
        while (1) {
            if (!is_prime((idx_t)next_candidate)) {
                /* It's a composite number (superior to 1) */
                if (composites_len + 1 > composites_cap) {
                    size_t new_cap = composites_cap == 0 ? 16 : composites_cap * 2;
                    seq_t *tmp = mem_resize(seq_t, composites, new_cap);
                    composites = tmp;
                    composites_cap = new_cap;
                }
                composites[composites_len++] = (seq_t)next_candidate;
                /* Advance to the next candidate for future calls */
                next_candidate++;
                break;
            }
            next_candidate++;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) {
        /* Undefined for n < 1; return 0 as a harmless default */
        return (seq_t)0;
    }
    ensure_composites(n);
    seq_t c = composites[(size_t)n - 1];     /* nth composite number */
    return ipow(c, c);                    /* c^c */
}
