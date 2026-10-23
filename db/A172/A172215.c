/*
 * A172215  Number of ways to place 6 nonattacking knights on a 6 X n board.
 * https://oeis.org/A172215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 22, u64 n <= 813, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A172215: Number of ways to place 6 non‑attacking knights on a 6×n board
   For 1 ≤ n < 10 the sequence is defined by discrete values.
   For n ≥ 10 a closed polynomial holds:
      a(n) = (648 n⁶ – 11340 n⁵ + 103770 n⁴ – 606645 n³
              + 2328317 n² – 5466660 n + 6051720) / 10
   The polynomial is evaluated using a 128‑bit intermediate type when available
   to avoid overflow on 64‑bit builds.  If 128‑bit integers are not supported,
   64‑bit arithmetic is used; values beyond that range are not required to
   match 64‑bit outputs. */
seq_t a(idx_t n)
{
    /* values for 1 ≤ n ≤ 9 */
    static const seq_t first[10] = {
        0ULL,            /* unused: a(0) */
        1ULL,            /* a(1) */
        58ULL,           /* a(2) */
        729ULL,          /* a(3) */
        8830ULL,         /* a(4) */
        60285ULL,        /* a(5) */
        257318ULL,       /* a(6) */
        858262ULL,       /* a(7) */
        2404448ULL,      /* a(8) */
        5879329ULL       /* a(9) */
    };

    if (n < 10)
        return first[n];

    /* evaluate the polynomial for n ≥ 10 */
#ifdef __SIZEOF_INT128__
    unsigned __int128 t = (unsigned __int128)n;
    unsigned __int128 res = 648;
    res = res * t - 11340;
    res = res * t + 103770;
    res = res * t - 606645;
    res = res * t + 2328317;
    res = res * t - 5466660;
    res = res * t + 6051720;
    res /= (unsigned __int128)10;
    return (seq_t)res;
#else
    seq_t t = (seq_t)n;
    seq_t res = 648;
    res = res * t - 11340;
    res = res * t + 103770;
    res = res * t - 606645;
    res = res * t + 2328317;
    res = res * t - 5466660;
    res = res * t + 6051720;
    res /= 10;
    return res;
#endif
}
