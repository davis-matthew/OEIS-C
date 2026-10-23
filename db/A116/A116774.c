/*
 * A116774  Number of permutations of length n which avoid the patterns 2143,
 *          2341, 4312; or avoid the patterns 1234, 1432, 3412.
 * https://oeis.org/A116774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 - 3*n^5 + 10*n^4 - 15*n^3 + 79*n^2 - 72*n + 180)/180
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t   = (seq_t)n;
    seq_t n2  = t * t;
    seq_t n3  = n2 * t;
    seq_t n4  = n3 * t;
    seq_t n5  = n4 * t;
    seq_t n6  = n5 * t;

    seq_t num = n6;
    num = num - ((seq_t)3 * n5);
    num = num + ((seq_t)10 * n4);
    num = num - ((seq_t)15 * n3);
    num = num + ((seq_t)79 * n2);
    num = num - ((seq_t)72 * t);
    num = num + (seq_t)180;

    return num / (seq_t)180;
}
