/*
 * A172532  Number of ways to place 5 nonattacking knights on an n X n
 *          toroidal board.
 * https://oeis.org/A172532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^8 - 90*n^6 + 3395*n^4 - 64290*n^2 + 522504)/120,
 *            n≥10
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 15, u64 n <= 136, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed values for n = 1 .. 9 */
    static const seq_t tab[9] = {
        0ULL,        /* a(1) */
        0ULL,        /* a(2) */
        0ULL,        /* a(3) */
        128ULL,      /* a(4) */
        120ULL,      /* a(5) */
        30312ULL,    /* a(6) */
        283906ULL,   /* a(7) */
        1872064ULL,  /* a(8) */
        8643186ULL   /* a(9) */
    };

    if (n <= 9) return tab[n - 1];

    /* All computations are done in 128‑bit so that intermediate
       values never overflow on a 64‑bit build. */
    seq_t nn = (seq_t)n;
    unsigned __int128 N = (unsigned __int128)nn;

    unsigned __int128 n2 = N * N;          /* n^2  */
    unsigned __int128 n4 = n2 * n2;        /* n^4  */
    unsigned __int128 n6 = n4 * n2;        /* n^6  */
    unsigned __int128 n8 = n4 * n4;        /* n^8  */

    unsigned __int128 t =
        n8
        - (unsigned __int128)90 * n6
        + (unsigned __int128)3395 * n4
        - (unsigned __int128)64290 * n2
        + (unsigned __int128)522504;

    unsigned __int128 result = n2 * t / (unsigned __int128)120;

    return (seq_t)result;
}
