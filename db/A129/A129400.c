/*
 * A129400  Number of walks of length n on one 60-degree wedge of the
 *          equilateral triangular lattice. The walk can go along the walls of
 *          the wedge, but cannot cross the walls.
 * https://oeis.org/A129400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): (n+2)*a(n) = 2*(2*n+1)*a(n-1) + 12*(n-1)*a(n-2)
   with a(0)=1, a(1)=2. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a0 = (seq_t)1;  /* a(0) */
    seq_t a1 = (seq_t)2;  /* a(1) */
    seq_t ai;

    for (idx_t i = 2; i <= n; ++i) {
        /* Two branches are used only to keep the intermediate
           products within the limits of the type.  For 8‑byte
           types the compiler will optimise the test out. */
        if (sizeof(seq_t) <= 8) {
            unsigned long long k1 = 2ULL * (2ULL * i + 1ULL);   /* 2*(2*i+1) */
            unsigned long long k2 = 12ULL * (i - 1);           /* 12*(i-1) */

            /* Use 128‑bit intermediate to avoid overflow. */
            unsigned __int128 term1 = (unsigned __int128)a1 * k1;
            unsigned __int128 term2 = (unsigned __int128)a0 * k2;
            unsigned __int128 numer = term1 + term2;
            ai = (seq_t)(numer / (i + 2));
        } else {
            seq_t k1 = (seq_t)(2ULL * (2ULL * i + 1ULL));
            seq_t k2 = (seq_t)(12ULL * (i - 1));
            seq_t term1 = a1 * k1;
            seq_t term2 = a0 * k2;
            seq_t numer = term1 + term2;
            ai = numer / (i + 2);
        }

        a0 = a1;
        a1 = ai;
    }
    return a1;
}
