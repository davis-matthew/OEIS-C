/*
 * A027797  a(n) = 22*(n+1)*binomial(n+3,12).
 * https://oeis.org/A027797
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*(n+1)*binomial(n+3,12) (n ≥ 9)
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 22, u64 n <= 113,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>           /* for uint64_t and related types */

seq_t a(idx_t n) {
    /* For indices below the official offset (n < 9) the value is not defined
       in the OEIS but we return 0 for convenience. */
    if (n < 9)
        return 0;

    /* Compute binomial(n+3, 12) multiplicatively to avoid large
       intermediate factorials.  The calculation stays exact for any
       integer type seq_t that supports +, -, *, /, and % on the
       same bit‑width.                                 */
    idx_t const k = 12;
    idx_t a_val = n + 3;
    seq_t res = 1;

    /* Since n ≥ 9, a_val ≥ 12, so we can safely use k = 12. */
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(a_val - k + i));
        res = res / ((seq_t)i);          /* guaranteed integer division */
    }

    /* Final multiplication by (n+1) and by 22. */
    res = res * ((seq_t)n + 1);
    res = res * 22;

    return res;
}
