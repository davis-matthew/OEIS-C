/*
 * A008391  Coordination sequence for A_8 lattice.
 * https://oeis.org/A008391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0 a(n) = n * (715*n^6 + 6006*n^4 + 10395*n^2 +
 *            3044) / 280 otherwise (OEIS A008391)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 494, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    // All calculations performed in unsigned 128‑bit
    unsigned __int128 nn   = (unsigned __int128)n;
    unsigned __int128 n2   = nn * nn;          // n^2
    unsigned __int128 n4   = n2 * n2;          // n^4
    unsigned __int128 n6   = n4 * n2;          // n^6

    unsigned __int128 temp = 715ULL * n6
                           + 6006ULL * n4
                           + 10395ULL * n2
                           + 3044ULL;       // 715*n6 + ...

    unsigned __int128 num  = nn * temp;        // n * temp
    unsigned __int128 res  = num / 280ULL;     // divide by 280

    return (seq_t)res;
}
