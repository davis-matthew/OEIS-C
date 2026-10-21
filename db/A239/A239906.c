/*
 * A239906  Let cn(n,k) denote the number of times 11..1 (k 1's) appears in
 *          the binary representation of n; a(n) = n - cn(n,1) + cn(n,2) -
 *          cn(n,3).
 * https://oeis.org/A239906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - cn(n,1) + cn(n,2) - cn(n,3)
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* cn(n,k): number of occurrences of k consecutive 1-bits in binary representation of n. */
/* For k > 3 the terms are not included. */

seq_t a(idx_t n)
{
    idx_t cn1 = 0, cn2 = 0, cn3 = 0;
    idx_t run = 0;
    idx_t bits = n;

    while (bits != 0) {
        if (bits & 1u) {           /* current bit is 1 */
            ++run;
            ++cn1;
            if (run >= 2) ++cn2;
            if (run >= 3) ++cn3;
        } else {                    /* current bit is 0, run ends */
            run = 0;
        }
        bits >>= 1u;                /* shift to next bit */
    }

    return (seq_t)n - (seq_t)cn1 + (seq_t)cn2 - (seq_t)cn3;
}
