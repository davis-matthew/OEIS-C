/*
 * A159053  Indices n such that Fibonacci(n-3) is a multiple of n.
 * https://oeis.org/A159053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 636, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   fib_doubling_uint64, fib_mod_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A159053: indices n such that Fibonacci(n-3) is a multiple of n */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/fib_doubling_uint64.h"
#include "../utils/fib_mod_idx_t.h"

/* Main sequence function.  seq_t and idx_t are defined by the build. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    static seq_t *terms = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t current = 1;      /* next candidate to test */

    while (len < n) {
        uint64_t cand = (uint64_t)current;
        bool holds = false;
        /* cand is always > 0 because current starts at 1 */
        /* F(-2) ≡ 0 (mod 1) so n=1 is a solution */
        if (cand == 1) {
            holds = true;
        } else {
            int64_t k = (int64_t)cand - 3;
            uint64_t r = fib_mod_idx_t(k, cand);
            holds = (r % cand) == 0;
        }

        if (holds) {
            if (len + 1 > cap) {
                size_t newcap = cap ? cap << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failed – abort with zero value */
                    return 0;
                }
                terms = tmp;
                cap   = newcap;
            }
            terms[len++] = (seq_t)current;
        }
        current++;
    }
    return terms[n - 1];
}
