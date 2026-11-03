/*
 * A161843  a(n) = n-th composite plus n-th nonprime.
 * https://oeis.org/A161843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002808(n)+A141468(n) (nth composite + nth nonprime)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/*  a(n) = sum of the nth composite number and the nth nonprime number.
    The function memoises the sequences across calls and only
    generates as many terms as necessary. */
seq_t a(idx_t n)
{
    /* static cache for composites */
    static seq_t *composites = NULL;
    static size_t comp_cap   = 0;
    static idx_t  comp_len   = 0;

    /* static cache for nonprimes (including 0 and 1) */
    static seq_t *nonprimes = NULL;
    static size_t np_cap   = 0;
    static idx_t  np_len   = 0;

    /* next integer to test (start at 2) */
    static idx_t next_i = 2;

    /* initialise caches on first call */
    if (!composites) {
        comp_cap = 64;
        composites = mem_new(seq_t, comp_cap);
    }
    if (!nonprimes) {
        np_cap = 64;
        nonprimes = mem_new(seq_t, np_cap);
        /* nonprime sequence starts with 0, 1 */
        nonprimes[0] = (seq_t)0;
        nonprimes[1] = (seq_t)1;
        np_len = 2;
    }

    /* generate terms until both caches have at least *n* elements */
    while (comp_len < n || np_len < n) {
        bool prime = is_prime(next_i);

        if (!prime) {
            /* composites start with 4 */
            if (next_i >= 4) {
                if ((size_t)comp_len >= comp_cap) {
                    comp_cap *= 2;
                    composites = mem_resize(seq_t, composites, comp_cap);
                }
                composites[comp_len++] = (seq_t)next_i;
            }

            /* every non‑prime (composite ≥2) is added to the list */
            if (next_i >= 2) {
                if ((size_t)np_len >= np_cap) {
                    np_cap *= 2;
                    nonprimes = mem_resize(seq_t, nonprimes, np_cap);
                }
                nonprimes[np_len++] = (seq_t)next_i;
            }
        }

        ++next_i;
    }

    return composites[n - 1] + nonprimes[n - 1];
}
