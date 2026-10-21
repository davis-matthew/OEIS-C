/*
 * A068790  Number of strings over Z_4 of length n with trace 2 and subtrace
 *          2.
 * https://oeis.org/A068790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068790: Number of strings over Z₄ of length n with trace 2 and subtrace 2.
   a(n) is obtained by dynamic programming over the 16 possible (trace, subtrace) states.
   For a string of length n over Z₄ let a(n; t, s) be the number of such strings with
   trace ≡ t (mod 4) and sub‑trace ≡ s (mod 4).  A single digit d contributes
   trace d and sub‑trace 0.  Extending a string by a digit d transforms
       (t', s') → (t' + d mod 4 ,  s' + d·t' mod 4).
   Therefore
       a(n; t, s) = Σ_{d=0..3} a(n‑1; (t‑d) mod 4 , (s‑d·t′) mod 4 )
   where t′ = (t‑d) mod 4.  The desired sequence is a(n) = a(n; 2, 2). */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n < 1, but harmless */

    seq_t dp_prev[4][4];
    seq_t dp_cur[4][4];

    /* initialize for strings of length 1: one digit d gives trace d and sub‑trace 0 */
    for (int t = 0; t < 4; ++t)
        for (int s = 0; s < 4; ++s)
            dp_prev[t][s] = 0;
    for (int t = 0; t < 4; ++t)
        dp_prev[t][0] = 1;

    /* length 1 handled; loop from length 2 up to n */
    for (idx_t len = 2; len <= n; ++len) {
        for (int t = 0; t < 4; ++t) {
            for (int s = 0; s < 4; ++s) {
                seq_t val = 0;
                for (int d = 0; d < 4; ++d) {
                    int tprev = (t + 4 - d) % 4;
                    int spr   = (s + 4 - (d * tprev) % 4) % 4;
                    val += dp_prev[tprev][spr];
                }
                dp_cur[t][s] = val;
            }
        }
        /* copy current table to previous for next iteration */
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                dp_prev[i][j] = dp_cur[i][j];
    }

    return dp_prev[2][2];  /* a(n; 2, 2) */
}
