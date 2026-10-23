/*
 * A190305  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=csch(1), t=sech(1).
 * https://oeis.org/A190305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·sinh(1)⌋ + ⌊n·tanh(1)⌋
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* High‑precision fractional parts of sinh(1) and tanh(1) scaled by 10^19 */
    const seq_t FS = 1752011936438014569ULL;      /* floor(0.1752011936438014569 · 10^19) */
    const seq_t FT = 7615941559557648881ULL;      /* floor(0.7615941559557648881 · 10^19) */
    const seq_t SCALE = 10000000000000000000ULL;  /* 10^19 */

    seq_t n_seq = (seq_t)n;

    /* Compute floor(n·sinh(1)) = n + floor(n·FS/SCALE) */
    seq_t term_s = (seq_t)((unsigned __int128)n_seq * FS / SCALE);
    /* Compute floor(n·tanh(1)) = floor(n·FT/SCALE)   */
    seq_t term_t = (seq_t)((unsigned __int128)n_seq * FT / SCALE);

    return n_seq * 2 + term_s + term_t;
}
