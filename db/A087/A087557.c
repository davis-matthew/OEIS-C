/*
 * A087557  a(n) = smallest prime that is the concatenation of two numbers
 *          whose product is n.
 * https://oeis.org/A087557
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest prime that is the concatenation of two numbers
 *            whose product is n‑1. For n < 2 the function returns 0. If no
 *            such prime exists, the value 0 is returned. This matches the
 *            OEIS table with offset 2 (a(2) = 11, a(3) = 0, ...). The
 *            implementation is purely arithmetic and does not rely...
 * Range:     u8 n <= 26, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   cmp_seq_t, concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/concat_parts.h"   /* seq_t concat_parts(idx_t a, idx_t b); */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t); */
#include "../utils/cmp_seq_t.h"     /* int cmp_seq_t(const seq_t *, const seq_t *); */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 2.  Return 0 otherwise. */
    if (n < 2)
        return 0;

    /* Target product is n‑1. */
    idx_t m = n - 1;

    seq_t best = 0;                       /* 0 means “none seen yet”. */

    /* Enumerate all divisor pairs d * e = m. */
    /* The loop limits to sqrt(m) using division to avoid overflow. */
    for (idx_t d = 1; d <= m / d; ++d) {
        if (m % d != 0)
            continue;
        idx_t e = m / d;

        /* Candidate 1: concat(d, e). */
        seq_t cand1 = concat_parts(d, e);
        if (is_prime_big(cand1)) {
            if (best == 0 || cmp_seq_t(&cand1, &best) < 0)
                best = cand1;
        }

        /* Candidate 2: concat(e, d) if d != e. */
        if (d != e) {
            seq_t cand2 = concat_parts(e, d);
            if (is_prime_big(cand2)) {
                if (best == 0 || cmp_seq_t(&cand2, &best) < 0)
                    best = cand2;
            }
        }
    }

    return best;   /* 0 if no prime concatenation was found. */
}
