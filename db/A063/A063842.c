/*
 * A063842  Number of colorings of K_4 using at most n colors.
 * https://oeis.org/A063842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 6*n^5 + 24*n^4 + 56*n^3 + 83*n^2 + 70*n + 24) / 24
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 67, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow on the intermediate
       polynomial before division by 24.  This works for the 64‑bit
       build and also for arbitrary‑precision builds as the final
       value fits in the specified seq_t. */
    unsigned __int128 nn   = n;
    unsigned __int128 n2   = nn * nn;          // n^2
    unsigned __int128 n3   = n2 * nn;          // n^3
    unsigned __int128 n4   = n3 * nn;          // n^4
    unsigned __int128 n5   = n4 * nn;          // n^5
    unsigned __int128 n6   = n5 * nn;          // n^6

    unsigned __int128 val =
          n6
        + (unsigned __int128)6  * n5
        + (unsigned __int128)24 * n4
        + (unsigned __int128)56 * n3
        + (unsigned __int128)83 * n2
        + (unsigned __int128)70 * nn
        + (unsigned __int128)24;              // constant term

    val /= 24;  /* division is exact for all n ≥ 0 */

    return (seq_t)val;
}
