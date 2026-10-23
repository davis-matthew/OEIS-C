/*
 * A032094  Number of reversible strings with n-1 beads of 2 colors. 7 beads
 *          are black. String is not palindromic.
 * https://oeis.org/A032094
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (n^7-28*n^6+322*n^5-1960*n^4+6664*n^3-11872*n^2+8448*n)/10080
 *            for even n;
 *            (n^7-28*n^6+322*n^5-1960*n^4+6769*n^3-13132*n^2+13068*n-5040)/10080
 *            for odd n; a(n)=0 for n<9
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 92, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 9) return 0;
    /* use unsigned __int128 for intermediate arithmetic to avoid overflow */
    unsigned __int128 N  = (unsigned __int128) n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;
    unsigned __int128 n5 = n4 * N;
    unsigned __int128 n6 = n5 * N;
    unsigned __int128 n7 = n6 * N;

    unsigned __int128 pos, neg, numer;
    if ((n & 1U) == 0) {             /* even n */
        pos = n7 + (unsigned __int128)322 * n5
              + (unsigned __int128)6664 * n3
              + (unsigned __int128)8448 * N;
        neg = (unsigned __int128)28 * n6
              + (unsigned __int128)1960 * n4
              + (unsigned __int128)11872 * n2;
    } else {                         /* odd n */
        pos = n7 + (unsigned __int128)322 * n5
              + (unsigned __int128)6769 * n3
              + (unsigned __int128)13068 * N;
        neg = (unsigned __int128)28 * n6
              + (unsigned __int128)1960 * n4
              + (unsigned __int128)13132 * n2
              + (unsigned __int128)5040;
    }
    numer = pos - neg;
    return (seq_t)(numer / 10080);
}
