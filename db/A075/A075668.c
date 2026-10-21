/*
 * A075668  Sum of next n 7th powers.
 * https://oeis.org/A075668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^15 + 42*n^13 + 168*n^11 + 206*n^9 - 11*n^7 - 56*n^5
 *            + 32*n^3)/384
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 26, u128 n <= 510,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#ifdef __cplusplus
    if constexpr (sizeof(seq_t) <= 8) {
        unsigned __int128 nd = n;
        unsigned __int128 n2 = nd * nd;
        unsigned __int128 n3 = n2 * nd;
        unsigned __int128 n5 = n3 * n2;
        unsigned __int128 n7 = n5 * n2;
        unsigned __int128 n9 = n7 * n2;
        unsigned __int128 n11 = n9 * n2;
        unsigned __int128 n13 = n11 * n2;
        unsigned __int128 n15 = n13 * n2;

        unsigned __int128 num =
              (unsigned __int128)3 * n15
            + (unsigned __int128)42 * n13
            + (unsigned __int128)168 * n11
            + (unsigned __int128)206 * n9
            - (unsigned __int128)11 * n7
            - (unsigned __int128)56 * n5
            + (unsigned __int128)32 * n3;

        return (seq_t)(num / 384);
    } else {
        seq_t nseq = (seq_t)n;
        seq_t n2 = nseq * nseq;
        seq_t n3 = n2 * nseq;
        seq_t n5 = n3 * n2;
        seq_t n7 = n5 * n2;
        seq_t n9 = n7 * n2;
        seq_t n11 = n9 * n2;
        seq_t n13 = n11 * n2;
        seq_t n15 = n13 * n2;

        seq_t num =
            (seq_t)3 * n15
          + (seq_t)42 * n13
          + (seq_t)168 * n11
          + (seq_t)206 * n9
          - (seq_t)11 * n7
          - (seq_t)56 * n5
          + (seq_t)32 * n3;

        return num / (seq_t)384;
    }
#else
    unsigned __int128 nd = n;
    unsigned __int128 n2 = nd * nd;
    unsigned __int128 n3 = n2 * nd;
    unsigned __int128 n5 = n3 * n2;
    unsigned __int128 n7 = n5 * n2;
    unsigned __int128 n9 = n7 * n2;
    unsigned __int128 n11 = n9 * n2;
    unsigned __int128 n13 = n11 * n2;
    unsigned __int128 n15 = n13 * n2;

    unsigned __int128 num =
          (unsigned __int128)3 * n15
        + (unsigned __int128)42 * n13
        + (unsigned __int128)168 * n11
        + (unsigned __int128)206 * n9
        - (unsigned __int128)11 * n7
        - (unsigned __int128)56 * n5
        + (unsigned __int128)32 * n3;

    return (seq_t)(num / 384);
#endif
}
