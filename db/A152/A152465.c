/*
 * A152465  Degrees of irreducible representations of SL(2,9).
 * https://oeis.org/A152465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) gives the sorted list of irreducible character degrees of
 *            SL(2,9). The degrees are: 1, 4, 4, 5, 5, 8, 8, 8, 8, 9, 10, 10,
 *            10. The list contains 13 terms; a(n) = 0 for n > 13 (undefined)
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ull)          return 0;          /* below offset – undefined */
    if (n == 1ull)          return (seq_t)1;   /* 1st term */
    if (n <= 3ull)          return (seq_t)4;   /* 2nd–3rd terms */
    if (n <= 5ull)          return (seq_t)5;   /* 4th–5th terms */
    if (n <= 9ull)          return (seq_t)8;   /* 6th–9th terms */
    if (n <= 10ull)         return (seq_t)9;   /* 10th term */
    if (n <= 13ull)         return (seq_t)10;  /* 11th–13th terms */
    return 0;                                    /* beyond the defined sequence */
}
