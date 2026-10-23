/*
 * A117248  Number of down steps at start of segment n of A079051.
 * https://oeis.org/A117248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=0, a(2)=0, a(3)=1. For n ≥ 4: if n is even, a(n) =
 *            n-2 if n is odd, a(n) = (n+1)/2 This matches the description
 *            “interleaving of 3,4,5,6,… and 0,2,4,6,8,…”
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)               /* a(0), a(1), a(2) */
        return 0;
    if (n == 3)              /* a(3) */
        return 1;

    if ((n & 1ULL) == 0) {   /* even n ≥ 4 */
        return (seq_t)(n - 2);
    } else {                 /* odd n ≥ 5 */
        return (seq_t)((n + 1) / 2);
    }
}
