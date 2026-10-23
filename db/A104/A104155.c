/*
 * A104155  The 64 codons of the genetic code, giving the value 1 to thymine
 *          (T), 3 to adenine (A), 2 to cytosine (C) and 4 to guanine (G).
 * https://oeis.org/A104155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   a(n) = 100*( ((n-1)/16)%4 + 1 ) + 10*( ((n-1)/4)%4 + 1 ) + (
 *            (n-1)%4 + 1 ) i.e., write (n-1) in base‑4 with three digits
 *            (0..3), map 0→1, 1→2, 2→3, 3→4, and interpret those three digits
 *            as decimal digits to form a three‑digit integer
 * Range:     u8 n <= 32, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n - 1;          /* (n-1) */
    idx_t d0 = (m / 16) % 4;  /* most significant base‑4 digit (0..3) */
    idx_t d1 = (m / 4) % 4;   /* middle digit             (0..3) */
    idx_t d2 = m % 4;         /* least significant digit  (0..3) */

    seq_t res = (seq_t)100 * ((seq_t)d0 + 1)
              + (seq_t)10  * ((seq_t)d1 + 1)
              +      ((seq_t)d2 + 1);
    return res;
}
