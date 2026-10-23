/*
 * A010127  Continued fraction for sqrt(23).
 * https://oeis.org/A010127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 8120, u16 n <= 8120, u32 n <= 8120, u64 n <= 8120,
 *            u128 n <= 8120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010127 – continued fraction coefficients for sqrt(23).
   Formula: a(0)=4; for n>0:
     a(n)=8  if n % 4 == 0,
     a(n)=3  if n % 4 == 2,
     a(n)=1  if n % 4 == 1 or 3. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;

    /* Compute n modulo 4.  Using bitwise & works for all n. */
    idx_t r = n & 3ULL;

    switch (r) {
        case 0: return (seq_t)8;
        case 1: return (seq_t)1;
        case 2: return (seq_t)3;
        case 3: return (seq_t)1;
    }
    /* Unreachable, but keeps the compiler satisfied. */
    return (seq_t)0;
}
