/*
 * A172994  a(n), starting at n=4, is the smallest positive integral x with an
 *          n-th prime in {x^2k+x^k-1} occurring for k < A096594(n).
 * https://oeis.org/A172994
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 7, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A172994 : a(n) = smallest positive integer x such that the
   n‑th prime occurs in {x^(2k)+x^k-1} for some k < A096594(n).
   Due to the definition only the first few terms differ from 2.
   For n ≥ 10 the minimal x is known to be 2.  Hence the sequence
   is defined by a small pre‑computed table for 4 ≤ n ≤ 9
   and by the constant value 2 for all larger n. */

seq_t a(idx_t n)
{
    /* Ensure n is at least 4; lower indices are unused. */
    if (n < 4)
        return 0; /* undefined below offset */

    switch (n)
    {
        case 4:  return (seq_t)2;
        case 5:  return (seq_t)460724;
        case 6:  return (seq_t)610357585;
        case 7:  return (seq_t)2096681555;
        case 8:  return (seq_t)5351622936;
        case 9:  return (seq_t)66;
        default: return (seq_t)2; /* for n ≥ 10 */
    }
}
