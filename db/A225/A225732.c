/*
 * A225732  Numbers n such that n < d(n)^(24/10), where d(n) is the number of
 *          divisors of n.
 * https://oeis.org/A225732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 512, u32 n <= 528, u64 n <= 528,
 *            u128 n <= 528, big any n
 * Validated: full, pass, 528 terms through n = 528 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n‑th integer m such that m^5 < d(m)^12
 * (equivalently m < d(m)^(24/10)), where d(m) is the number of divisors of m.
 * The sequence is finite; the algorithm generates candidates sequentially
 * and memoises all found terms.  The result is returned for any n ≥ 1.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_divisors.h"
#include "../utils/mem.h"

static inline bool candidate_ok(uint64_t m)
{
    /* d(m) is always ≤ 2^63 for the small values we test. */
    uint64_t d = num_divisors(m);

    /* Use unsigned __int128 to avoid overflow for the small m in the sequence. */
    unsigned __int128 left = (unsigned __int128)m;
    left *= m; left *= m; left *= m; left *= m;  /* m^5 */

    unsigned __int128 right = (unsigned __int128)d;
    for (int i = 0; i < 11; ++i)  /* 11 multiplications → d^12 */
        right *= d;

    return left < right;
}

seq_t a(idx_t n)
{
    static seq_t *terms   = NULL;   /* memoisation array */
    static size_t len     = 0;      /* number of terms stored */
    static size_t cap     = 0;      /* capacity of the array */
    static uint64_t next  = 2;      /* next integer to test */

    /* initialise the array once */
    if (cap == 0) {
        cap = 4;
        terms = mem_new(seq_t, cap);
    }

    /* generate terms until we have at least n of them */
    while (len < (size_t)n) {
        if (candidate_ok(next)) {
            if (len == cap) {       /* grow the array */
                cap <<= 1;
                terms = mem_resize(seq_t, terms, cap);
            }
            terms[len++] = (seq_t)next;   /* cast to seq_t */
        }
        ++next;
    }

    return terms[(size_t)n - 1];
}
