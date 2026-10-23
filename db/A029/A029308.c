/*
 * A029308  Expansion of 1/((1-x^3)*(1-x^7)*(1-x^8)*(1-x^11)).
 * https://oeis.org/A029308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((1414*n^3+1029*n^2+1596*n+2986)/2016) -
 *            floor((3*n^3+n^2+2*n+4)/7) - floor((3*n^3+4*n^2+5*n+10)/11)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for intermediate values to avoid overflow
       when seq_t is 64‑bit.  All constants fit in 128‑bit.  The
       expression is evaluated exactly and then cast to seq_t. */
    unsigned __int128 n128 = n;

    unsigned __int128 t1 = (unsigned __int128)1414 * n128 * n128 * n128
                        + (unsigned __int128)1029 * n128 * n128
                        + (unsigned __int128)1596 * n128
                        + (unsigned __int128)2986;
    seq_t term1 = (seq_t)(t1 / 2016);

    unsigned __int128 t2 = (unsigned __int128)3 * n128 * n128 * n128
                        + (unsigned __int128)1 * n128 * n128
                        + (unsigned __int128)2 * n128
                        + (unsigned __int128)4;
    seq_t term2 = (seq_t)(t2 / 7);

    unsigned __int128 t3 = (unsigned __int128)3 * n128 * n128 * n128
                        + (unsigned __int128)4 * n128 * n128
                        + (unsigned __int128)5 * n128
                        + (unsigned __int128)10;
    seq_t term3 = (seq_t)(t3 / 11);

    return term1 - term2 - term3;
}
