/*
 * A008389  Coordination sequence for A_7 lattice.
 * https://oeis.org/A008389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + n^2*(143*n^4 + 770*n^2 + 707)/30 (a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 31, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Use 128‑bit intermediate to avoid overflow for almost all n */
    unsigned __int128 nn  = (unsigned __int128)n;
    unsigned __int128 n2  = nn * nn;
    unsigned __int128 n4  = n2 * n2;
    unsigned __int128 inner = 143 * n4 + 770 * n2 + 707;
    unsigned __int128 result = (n2 * inner) / 30 + 2;

    return (seq_t)result;
}
