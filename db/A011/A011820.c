/*
 * A011820  Number of M-sequences m_0,...,m_4 with m_1 < n.
 * https://oeis.org/A011820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^10 +61*n^9 +408*n^8 +1722*n^7 +6132*n^6 +18333*n^5
 *            +41072*n^4 +88748*n^3 +133824*n^2 +193536*n +241920)/120960
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 24, u64 n <= 235, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the same type as the result */
    seq_t N = (seq_t)n;

#if defined(__SIZEOF_INT128__)
    /* Use 128‑bit arithmetic to hold the intermediate values safely. */
    unsigned __int128 v = (unsigned __int128)4;
    v = v * (unsigned __int128)N + (unsigned __int128)61;
    v = v * (unsigned __int128)N + (unsigned __int128)408;
    v = v * (unsigned __int128)N + (unsigned __int128)1722;
    v = v * (unsigned __int128)N + (unsigned __int128)6132;
    v = v * (unsigned __int128)N + (unsigned __int128)18333;
    v = v * (unsigned __int128)N + (unsigned __int128)41072;
    v = v * (unsigned __int128)N + (unsigned __int128)88748;
    v = v * (unsigned __int128)N + (unsigned __int128)133824;
    v = v * (unsigned __int128)N + (unsigned __int128)193536;
    v = v * (unsigned __int128)N + (unsigned __int128)241920;
    return (seq_t)(v / 120960);
#else
    /* If 128‑bit types are not available, rely on the arithmetic of seq_t
       (which may be an arbitrary‑precision type in the build) to avoid
       overflow. */
    seq_t v = (seq_t)4;
    v = v * N + (seq_t)61;
    v = v * N + (seq_t)408;
    v = v * N + (seq_t)1722;
    v = v * N + (seq_t)6132;
    v = v * N + (seq_t)18333;
    v = v * N + (seq_t)41072;
    v = v * N + (seq_t)88748;
    v = v * N + (seq_t)133824;
    v = v * N + (seq_t)193536;
    v = v * N + (seq_t)241920;
    return v / 120960;
#endif
}
