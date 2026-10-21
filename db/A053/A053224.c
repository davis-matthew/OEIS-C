/*
 * A053224  Numbers k for which sigma(k) < sigma(k+1).
 * https://oeis.org/A053224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes sequence A053224: numbers k for which σ(k) < σ(k+1).
 *  The implementation scans k sequentially, caching results so that
 *  any n ≥ 1 can be answered in O(n) time (≈2 k operations per call).
 *  It is compatible with C11 and C++17 and uses only the allowed
 *  operators on seq_t.  The function is fully memoised: calls may
 *  arrive in any order.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sigma_u64.h"

/* Return the n‑th term of A053224 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* static cache of found terms */
    static seq_t *terms   = NULL;
    static size_t  tcap   = 0;
    static size_t  tlen   = 0;

    /* next candidate k to examine */
    static uint64_t next_k = 1;

    /* generate terms until we have at least n of them */
    while (tlen < (size_t)n) {
        uint64_t k = next_k;
        seq_t    s_k  = sigma_u64(k);
        seq_t    s_k1 = sigma_u64(k + 1);

        if (s_k < s_k1) {
            /* grow cache if necessary */
            if (tlen == tcap) {
                size_t newcap = tcap ? tcap * 2 : 8;
                seq_t *tmp = (seq_t*)realloc(terms, newcap * sizeof(seq_t));
                terms = tmp;
                tcap = newcap;
            }
            terms[tlen++] = (seq_t)k;
        }
        ++next_k;
    }

    return terms[n - 1];
}
