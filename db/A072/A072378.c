/*
 * A072378  Numbers n such that 12*n divides F(12*n), where F(m) is the m-th
 *          Fibonacci number.
 * https://oeis.org/A072378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number k such that 12*k | Fib(12*k) Computed by search
 *            with fast doubling for Fibonacci modulo m (m = 12*k). The first
 *            24 terms are:
 *            1,2,3,4,5,6,8,9,10,12,14,15,16,18,20,24,25,27,28,30,32,36,40,42
 * Range:     u8 n <= 88, u16 n <= 2795, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fib_mod_uint64, fib_pair_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib_mod_uint64.h"
#include "../utils/fib_pair_uint64.h"

/* Check whether 12*k divides Fib(12*k). */
static inline bool good_candidate(uint64_t k)
{
    uint64_t mod = k * 12ULL;            /* m = 12*k  */
    return fib_mod_uint64(mod, mod) == 0ULL;
}

/* Public function: index n (≥1) → n‑th term of seq A072378. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, but harmless */
    uint64_t need = (uint64_t)n;
    uint64_t found = 0;
    uint64_t cand   = 1;

    while (true) {
        if (good_candidate(cand)) {
            ++found;
            if (found == need)
                return (seq_t)cand;
        }
        ++cand;
    }
}
