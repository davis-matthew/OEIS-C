/*
 * A102700  Numbers k such that 10*k + 9 is prime.
 * https://oeis.org/A102700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9338 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching for k with 10*k+9 prime.
   a(n) is the n‑th such k (offset 1). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* NULL checks for index below offset */
    if (n == 0) return 0;   /* undefined for n < 1 */

    /* static cache: terms[0..size-1] hold already found values */
    static seq_t *terms = NULL;
    static size_t  size  = 0;   /* number of stored terms            */
    static size_t  cap   = 0;   /* allocated capacity                 */
    static uint64_t k_next = 1; /* next k candidate to test           */

    /* Initialise cache on first call */
    if (terms == NULL) {
        cap   = 128;
        terms = mem_new(seq_t, cap);
        if (terms == NULL) return 0;   /* memory exhaustion – abort silently */
    }

    /* Generate terms until we have at least n of them */
    while (size < n) {
        uint64_t candidate = 10ULL * k_next + 9ULL;
        if (is_prime_u64(candidate)) {
            /* grow array if necessary */
            if (size == cap) {
                size_t new_cap = cap << 1;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (tmp == NULL) return 0;   /* memory exhaustion – abort silently */
                terms = tmp;
                cap   = new_cap;
            }
            terms[size++] = (seq_t)k_next;
        }
        ++k_next;
    }

    return terms[n - 1];
}
