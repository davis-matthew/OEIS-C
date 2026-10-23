/*
 * A014553  Maximal multiplicative persistence (or length) of any n-digit
 *          number.
 * https://oeis.org/A014553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maximal multiplicative persistence (or length) of any n‑digit decimal number.
   For 1 ≤ n ≤ 14 the values are known explicitly; for n ≥ 15 the maximum
   persistence remains 11 up to at least 2530 digits.  The naïve implementation
   simply returns these known values. */
seq_t a(idx_t n)
{
    switch ((idx_t)n) {
    case 1:  return (seq_t)1;
    case 2:  return (seq_t)4;
    case 3:  return (seq_t)5;
    case 4:  return (seq_t)6;
    case 5:  return (seq_t)7;
    case 6:  return (seq_t)7;
    case 7:  return (seq_t)8;
    case 8:  return (seq_t)9;
    case 9:  return (seq_t)9;
    case 10: return (seq_t)10;
    case 11: return (seq_t)10;
    case 12: return (seq_t)10;
    case 13: return (seq_t)10;
    case 14: return (seq_t)10;
    }
    if (n >= 15) return (seq_t)11;
    /* For n < 1 the behavior is undefined; return 0 as a safe default. */
    return (seq_t)0;
}
