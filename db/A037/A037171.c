/*
 * A037171  Numbers n such that phi(n) = pi(n), i.e., A000010(n) = A000720(n).
 * https://oeis.org/A037171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: full, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   mem, prime_pi, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches for all integers n such that φ(n) = π(n)
   and stores them in a static array.  The sequence is finite (the
   last element is 90); for indices larger than the number of found
   elements the function simply returns 0. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/prime_pi.h"
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static storage for cached solutions. */
    static bool      initialized = false;
    static seq_t    *solutions   = NULL;
    static size_t    sol_cnt     = 0;
    static size_t    sol_cap     = 0;

    if (!initialized) {
        initialized = true;
        sol_cap = 16;                      /* initial capacity */
        solutions = mem_new(seq_t, sol_cap);
        sol_cnt = 0;

        /* Search up to a value comfortably above the largest known
           solution (90).  The sequence is finite, so any bound larger
           than 90 is sufficient. */
        for (idx_t cur = 2; cur <= 1000; ++cur) {
            seq_t cur_seq = (seq_t)cur;
            seq_t phi_val = totient_seq(cur_seq);
            seq_t pi_val  = prime_pi(cur);
            if (phi_val == pi_val) {
                if (sol_cnt >= sol_cap) {
                    sol_cap <<= 1;               /* double capacity */
                    solutions = mem_resize(seq_t, solutions, sol_cap);
                }
                solutions[sol_cnt++] = cur_seq;
            }
        }
    }

    if (n == 0) return 0;            /* undefined for n < 1 */

    size_t idx = (size_t)(n - 1);    /* zero‑based index */
    if (idx < sol_cnt) return solutions[idx];

    /* If the requested index exceeds the number of known terms we
       simply return 0. */
    return 0;
}
