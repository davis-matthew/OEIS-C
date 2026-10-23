/*
 * A026052  (d(n)-r(n))/2, where d = A008778 and r is the periodic sequence
 *          with fundamental period (1,1,0,1).
 * https://oeis.org/A026052
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((m + 2)*(m^2 + 10*m + 15))/12 - 3/8 + 1/8*(-1)^m -
 *            1/4*cos(m*π/2), where m = n−2 (offset 2)
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* below offset – value irrelevant */

    idx_t m  = n - 2;             /* shift to 0‑based index */
    seq_t ms = (seq_t)m;

    /* (m+2) * (m^2 + 10*m + 15) * 2  –  all integer arithmetic */
    seq_t t      = ms * ms + ((seq_t)10) * ms + (seq_t)15;
    seq_t part1  = (ms + (seq_t)2) * t * (seq_t)2;

    /* (-1)^m part: s1 = 1 for even m, -1 for odd m */
    int s1 = (m & 1U) ? -1 : 1;

    /* cos(m*π/2) cycle: 1, 0, –1, 0 …  →  s2 = –cos(...)  */
    int r  = m & 3U;   /* m % 4 */
    int s2 = 0;
    if (r == 0) s2 = -1;   /* cos = 1  →  –1  */
    else if (r == 2) s2 = 1;/* cos = –1 →   1  */

    /* Numerator of the final division by 24 */
    seq_t num = part1 - (seq_t)9 + (seq_t)(3 * s1) + (seq_t)(6 * s2);

    /* Final result */
    seq_t res = num / (seq_t)24;
    return res;
}
