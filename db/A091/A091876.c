/*
 * A091876  Square roots of squares in A068177.
 * https://oeis.org/A091876
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 6 for n = 2 a(n) = 19 for n = 3 a(n) = 181 for n = 4 a(n)
 *            = 569 for n = 5 a(n) = 5 × 10^(n‑3) + 6 × (10^(n‑4) + … + 10) +
 *            9 which is equivalent to a(n) = 5, then (n‑4) copies of the
 *            digit 6, then the digit 9 (for n ≥ 6). The sequence gives the
 *            square roots of the squares listed in A068177
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first few special cases directly */
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)19;
    if (n == 4) return (seq_t)181;
    if (n == 5) return (seq_t)569;
    if (n < 2) return (seq_t)0;   /* undefined for n < 2 */

    /* build the number digit by digit:
       start with 5
       append (n-4) sixes (each time: val = val * 10 + 6)
       finally append a 9                                            */
    seq_t val = (seq_t)5;
    idx_t sixes = n - 4;
    for (idx_t i = 0; i < sixes; ++i) {
        val = val * (seq_t)10 + (seq_t)6;
    }
    val = val * (seq_t)10 + (seq_t)9;
    return val;
}
