/*
 * A194825  Digital roots of the nonzero 9-gonal (nonagonal) numbers.
 * https://oeis.org/A194825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = digital_root((7*n*n - 5*n)/2)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The digital root of the nth nonzero 9‑gonal number can be   */
/* expressed modulo 9, using the fact that 2⁻¹ ≡ 5 (mod 9).   */
seq_t a(idx_t n)
{
    /* compute n mod 9 */
    int r = (int)(n % 9);
    /* r² mod 9 */
    int r2 = (r * r) % 9;
    /* numerator modulo 9: 7*r² - 5*r */
    int num = (7 * r2) % 9;
    int sub = (5 * r) % 9;
    int num_mod = num - sub;
    if (num_mod < 0) 
        num_mod += 9;          /* ensure non‑negative before dividing by 2 */
    /* divide by 2 modulo 9: multiply by inverse of 2 (which is 5) */
    int t = (num_mod * 5) % 9;
    /* digital root: 9 if congruent to 0 modulo 9, else the remainder */
    return (t == 0) ? 9 : (seq_t)t;
}
