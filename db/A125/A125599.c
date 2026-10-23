/*
 * A125599  Quotient ((2n)^(2n-2)-1)/(2n+1)/(2n-1).
 * https://oeis.org/A125599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A125599
   a(n) = ((2n)^(2n-2)-1)/(2n+1)/(2n-1)
   Equivalent: a(n) = sum_{k=0}^{n-2} b^k  where b = (2n)^2
*/
seq_t a(idx_t n)
{
    if (n <= 1) return 0;        /* a(1) = 0 as given */

    /* b = (2n)^2 */
    seq_t two_n = (seq_t)n + (seq_t)n;
    seq_t b = two_n * two_n;

    /* n-1 terms (k = 0 .. n-2) */
    idx_t m = n - 1;

    /* For small seq_t types (≤ 64 bit) use 128‑bit intermediate
       to avoid overflow into the result. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pow = 1;
        unsigned __int128 sum = 0;
        for (idx_t i = 0; i < m; ++i) {
            sum += pow;
            pow *= (unsigned __int128)b;
        }
        return (seq_t)sum;
    } else {
        /* For larger or arbitrary‑precision seq_t,
           direct multiplication is safe. */
        seq_t pow = 1;
        seq_t sum = 0;
        for (idx_t i = 0; i < m; ++i) {
            sum += pow;
            pow *= b;
        }
        return sum;
    }
}
