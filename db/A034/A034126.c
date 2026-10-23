/*
 * A034126  Decimal part of cube root of a(n) starts with 0: first term of
 *          runs (cubes excluded).
 * https://oeis.org/A034126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n + 1)^3 + 1
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS sequence A034126.
   a(1) = 9, a(2) = 28, a(3) = 65, …   (offset 1)          */
seq_t a(idx_t n)
{
    seq_t nn = ((seq_t)n) + 1;   /* n >= 1, cast to seq_t first */
    seq_t t  = nn * nn;          /* nn^2 */
    t = t * nn;                  /* nn^3 */
    t += 1;                      /* +1 */
    return t;
}
