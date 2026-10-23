/*
 * A126919  Coordination sequence for 12-dimensional cyclotomic lattice
 *          Z[zeta_26].
 * https://oeis.org/A126919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (13*n^11 + 715*n^9 + 21879*n^7 + 188045*n^5 + 594308*n^3
 *            + 318240*n) / 43200 (n>0), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 15, u64 n <= 117, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }

    /* use 128‑bit arithmetic for the intermediate steps
       to avoid overflow for all n whose a(n) fits in seq_t */
    unsigned __int128 N  = n;
    unsigned __int128 t1 = 13        * N * N * N * N * N * N * N * N * N * N * N; // n^11
    unsigned __int128 t2 = 715       * N * N * N * N * N * N * N * N * N;          // n^9
    unsigned __int128 t3 = 21879     * N * N * N * N * N * N * N;                   // n^7
    unsigned __int128 t4 = 188045    * N * N * N * N * N;                          // n^5
    unsigned __int128 t5 = 594308    * N * N * N;                                 // n^3
    unsigned __int128 t6 = 318240    * N;                                         // n

    unsigned __int128 sum = t1 + t2 + t3 + t4 + t5 + t6;
    unsigned __int128 res  = sum / 43200;

    return (seq_t)res;
}
