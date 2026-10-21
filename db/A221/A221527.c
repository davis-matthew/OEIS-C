/*
 * A221527  Number of 0..n arrays of length 7 with each element differing from
 *          at least one neighbor by 2 or more.
 * https://oeis.org/A221527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + n^6 - 29*n^5 + 109*n^4 - 204*n^3 + 202*n^2 - 80*n
 *            (for n>2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Handle the offset terms explicitly
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)16;

    // Compute successive powers of n
    seq_t nn   = (seq_t)n;     // n^1
    seq_t n2   = nn * nn;     // n^2
    seq_t n3   = n2 * nn;     // n^3
    seq_t n4   = n3 * nn;     // n^4
    seq_t n5   = n4 * nn;     // n^5
    seq_t n6   = n5 * nn;     // n^6
    seq_t n7   = n6 * nn;     // n^7

    // Separate positive and negative contributions
    seq_t pos = n7 + n6 + 109 * n4 + 202 * n2;
    seq_t neg = 29 * n5 + 204 * n3 + 80 * nn;

    // For n > 2 the result is non‑negative
    return pos - neg;
}
