/*
 * A161989  Numbers having more than 2 or fewer than 2 ones in their binary
 *          representation.
 * https://oeis.org/A161989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 228, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A161989
    a(n) = nth non‑negative integer whose binary representation
    has a number of ones different from 2.
    (All numbers except those of the form 2^i + 2^j with i<j.)
    a(1) = 0
*/

seq_t a(idx_t n)
{
    idx_t  seen = 0;          /* how many qualifying numbers seen so far   */
    seq_t  x    = 0;          /* candidate number                          */
    for (;; ++x) {
        seq_t  v = x;
        seq_t  bits = 0;
        while (v) {                     /* count popcount of v */
            bits += (v & 1);
            v >>= 1;
        }
        if (bits != 2) {                /* keep if popcount≠2 */
            ++seen;
            if (seen == n)
                return x;
        }
    }
}
