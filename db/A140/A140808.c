/*
 * A140808  a(n) is the smallest composite of the form n*k - 1.
 * https://oeis.org/A140808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite of the form n*k - 1
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Returns the smallest composite number of the form n*k-1 with k ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* Undefined for n=0; safe fallback */
    seq_t nseq = (seq_t)n;             /* Promote n to seq_t for arithmetic */
    for (idx_t k = 1; ; ++k) {
        seq_t v = nseq * (seq_t)k - (seq_t)1;  /* Compute n*k - 1 */
        if (v <= (seq_t)1) continue;          /* 0 or 1 are not composite */
        if (!is_prime_big(v)) return v;        /* Found first composite */
    }
}
