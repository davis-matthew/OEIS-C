/*
 * A190397  Number of ways to place 5 nonattacking grasshoppers on a
 *          chessboard of size n x n.
 * https://oeis.org/A190397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/120*(n^10 -10*n^8 -200*n^7 +1175*n^6 -1136*n^5 -740*n^4
 *            -30520*n^3 +159624*n^2 -289024*n +179175 -135*(-1)^n), n>3
 *            a(1)=0, a(2)=0, a(3)=28
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 14, u64 n <= 136, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small values */
    if (n == 1 || n == 2) return 0;
    if (n == 3) return (seq_t)28;

#if defined(__SIZEOF_INT128__) || defined(__INT128__)
    /* Use 128‑bit integer for safe intermediate arithmetic */
    const unsigned __int128 N   = (unsigned __int128)n;
    const unsigned __int128 n2  = N * N;
    const unsigned __int128 n3  = n2 * N;
    const unsigned __int128 n4  = n3 * N;
    const unsigned __int128 n5  = n4 * N;
    const unsigned __int128 n6  = n5 * N;
    const unsigned __int128 n7  = n6 * N;
    const unsigned __int128 n8  = n7 * N;
    const unsigned __int128 n10 = n8 * n2;

    unsigned __int128 num = n10
        - (unsigned __int128)10 * n8
        - (unsigned __int128)200 * n7
        + (unsigned __int128)1175 * n6
        - (unsigned __int128)1136 * n5
        - (unsigned __int128)740 * n4
        - (unsigned __int128)30520 * n3
        + (unsigned __int128)159624 * n2
        - (unsigned __int128)289024 * N
        + 179175;

    /* add parity term  -135*(-1)^n */
    if (n & 1) num += 135;     /* n odd  → +135  */
    else       num -= 135;     /* n even → -135  */

    num /= 120;                /* division is exact */
    return (seq_t)num;
#else
    /* Fallback: use seq_t arithmetic. May overflow for very large n, but
       is sufficient for seq_t types that cannot handle 128‑bit values. */
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;
    seq_t n3  = n2 * N;
    seq_t n4  = n3 * N;
    seq_t n5  = n4 * N;
    seq_t n6  = n5 * N;
    seq_t n7  = n6 * N;
    seq_t n8  = n7 * N;
    seq_t n10 = n8 * n2;

    seq_t num = n10
        - (seq_t)10 * n8
        - (seq_t)200 * n7
        + (seq_t)1175 * n6
        - (seq_t)1136 * n5
        - (seq_t)740 * n4
        - (seq_t)30520 * n3
        + (seq_t)159624 * n2
        - (seq_t)289024 * N
        + (seq_t)179175;

    if (n & 1) num += (seq_t)135;
    else       num -= (seq_t)135;
    num /= (seq_t)120;
    return num;
#endif
}
