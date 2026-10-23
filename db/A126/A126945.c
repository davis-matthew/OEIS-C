/*
 * A126945  The sequence a[n]+b[n]+c[n]+d[n] defined in A126940.
 * https://oeis.org/A126945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function
   A126945:  a(0)=1, a(1)=5, a(2)=15, a(3)=47, a(4)=151, a(5)=479
   For n ≥ 6:
       a(n) = 2*a(n‑1) + a(n‑2) + 7*a(n‑3) + 4*a(n‑4) + 4*a(n‑5) + a(n‑6)
*/
seq_t a(idx_t n)
{
    /* initial values for n = 0…5 */
    static const seq_t init[6] = {1, 5, 15, 47, 151, 479};

    if (n < 6)
        return init[n];

    seq_t a0 = init[0], a1 = init[1], a2 = init[2],
          a3 = init[3], a4 = init[4], a5 = init[5];
    seq_t an;

    for (idx_t i = 6; i <= n; ++i) {
        an = 2 * a5 + a4 + 7 * a3 + 4 * a2 + 4 * a1 + a0;
        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }
    return an;
}
