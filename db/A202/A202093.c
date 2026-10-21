/*
 * A202093  Number of (n+2) X 3 binary arrays avoiding patterns 001 and 011 in
 *          rows and columns.
 * https://oeis.org/A202093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { (n^6 + 28 n^5 + 324 n^4 + 1984 n^3 + 6784 n^2 + 12288 n
 *            + 9216)/256 for n even * (n^6 + 28 n^5 + 321 n^4 + 1928 n^3 +
 *            6395 n^2 + 11100 n + 7875)/256 for n odd }
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 96, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 n128 = n;
    unsigned __int128 n2 = n128 * n128;
    unsigned __int128 n3 = n2 * n128;
    unsigned __int128 n4 = n3 * n128;
    unsigned __int128 n5 = n4 * n128;
    unsigned __int128 n6 = n5 * n128;
    unsigned __int128 den = 256;
    unsigned __int128 num;

    if ((n & 1) == 0) {
        num = n6
            + (unsigned __int128)28 * n5
            + (unsigned __int128)324 * n4
            + (unsigned __int128)1984 * n3
            + (unsigned __int128)6784 * n2
            + (unsigned __int128)12288 * n128
            + (unsigned __int128)9216;
    } else {
        num = n6
            + (unsigned __int128)28 * n5
            + (unsigned __int128)321 * n4
            + (unsigned __int128)1928 * n3
            + (unsigned __int128)6395 * n2
            + (unsigned __int128)11100 * n128
            + (unsigned __int128)7875;
    }

    return (seq_t)(num / den);
}
