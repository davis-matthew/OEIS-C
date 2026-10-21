/*
 * A072915  Decimal expansion of the standard gravity acceleration (one "gee")
 *          in SI units.
 * https://oeis.org/A072915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  full, fini, nonn, cons
 * Formula:   a(n) = the n-th decimal digit of 9.80665 (1‑indexed), a(1)=9,
 *            digits 2‑6 are 8,0,6,6,5 respectively, and a(n)=0 for n>6
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The value 9.80665 has exactly five decimal digits.
       All subsequent digits are zeros. */
    if      (n == 1) return (seq_t)9;
    else if (n == 2) return (seq_t)8;
    else if (n == 3) return (seq_t)0;
    else if (n == 4) return (seq_t)6;
    else if (n == 5) return (seq_t)6;
    else if (n == 6) return (seq_t)5;
    else             return (seq_t)0;
}
