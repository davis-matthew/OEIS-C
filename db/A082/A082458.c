/*
 * A082458  Multiply by 1, add 1, multiply by 2, add 2, etc., starting with 0.
 * https://oeis.org/A082458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 40, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A082458 – Multiply by 1, add 1, multiply by 2, add 2, …
 *
 *  a(0) = 0,  a(1) = 0
 *  For n >= 2 let m = ⌊n/2⌋.
 *      a(2m)   = floor(e · m!) – 1
 *      a(2m+1) = (m+1)·(floor(e · m!) – 1)
 *
 *  The integer values f(m)=floor(e · m!) satisfy
 *      f(1) = 2
 *      f(k) = k·f(k‑1) + 1 (for k ≥ 2).
 *  The recurrence is used directly – no floating point or large
 *  intermediate factorials are required.
 */

seq_t a(idx_t n)
{
    /* Base values */
    if (n <= 1) return 0;

    /* m = floor(n/2) */
    idx_t m = n >> 1;

    /* Compute f(m) = floor(e · m!) via recurrence. */
    seq_t f = 2;                     /* f(1) = 2 */
    /* For m == 1 the loop is skipped – f stays 2. */
    for (idx_t k = 2; k <= m; ++k) {
        f = k * f + 1;
    }

    /* Return according to parity of n. */
    if ((n & 1) == 0) {              /* even index : a(2m) */
        return f - 1;
    } else {                         /* odd index  : a(2m+1) */
        return (seq_t)(m + 1) * (f - 1);
    }
}
