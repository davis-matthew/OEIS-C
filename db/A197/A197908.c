/*
 * A197908  Ceiling((n+1/n)^9).
 * https://oeis.org/A197908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n + 1/n)^9) ≡ n^9 + 9·n^7 + 36·n^5 + 84·n^3 + 126·n
 *            + 1 + (126·n^8 + 84·n^6 + 36·n^4 + 9·n^2) / n^9
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 138, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined for n = 0 */

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;
    seq_t n7 = n6 * nn;
    seq_t n8 = n7 * nn;
    seq_t n9 = n8 * nn;

    seq_t part = n9
               + 9 * n7
               + 36 * n5
               + 84 * n3
               + 126 * nn
               + (126 * n8 + 84 * n6 + 36 * n4 + 9 * n2) / n9;

    return part + (seq_t)1;                  /* add the integer 1 from the ceil */
}
