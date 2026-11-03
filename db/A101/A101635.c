/*
 * A101635  Increasing primes of alternating congruences modulo 6.
 * https://oeis.org/A101635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Increasing primes of alternating congruences modulo 6
   a(2n) ≡ 1 (mod 6), a(2n+1) ≡ 5 (mod 6)
   The function builds the sequence incrementally with prime testing. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* static state for incremental construction */
    static seq_t *vals = NULL;        /* array of primes in the sequence */
    static idx_t sz = 0;              /* current allocated size */
    static idx_t cnt = 0;             /* number of primes stored */
    static uint64_t cand = 5;         /* next candidate to test */

    /* grow storage when needed */
    if (cnt == sz) {
        idx_t new_sz = sz == 0 ? 64 : sz << 1;
        seq_t *tmp = mem_resize(seq_t, vals, new_sz);
        if (!tmp) exit(EXIT_FAILURE); /* out of memory */
        vals = tmp;
        sz = new_sz;
    }

    /* generate primes until we have the nth term */
    while (cnt < n) {
        if (is_prime_u64(cand)) {
            if ((cand % 6ull == 5ull && (cnt % 2ull == 0ull)) ||
                (cand % 6ull == 1ull && (cnt % 2ull == 1ull))) {
                vals[cnt++] = (seq_t)cand;
                if (cnt == sz) {       /* may happen for the last added */
                    idx_t new_sz = sz << 1;
                    seq_t *tmp = mem_resize(seq_t, vals, new_sz);
                    if (!tmp) exit(EXIT_FAILURE);
                    vals = tmp;
                    sz = new_sz;
                }
            }
        }
        cand += 2ull; /* skip even numbers */
    }

    return vals[n - 1];
}
