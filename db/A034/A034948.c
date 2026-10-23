/*
 * A034948  Decimal expansion of 1/9801.
 * https://oeis.org/A034948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A034948 – decimal digits of 1/9801 (1/99^2).
 *
 * The decimal expansion is periodic with period 198 digits.
 * The 198 digits consist of 99 pairs of two digits:
 *   00, 01, 02, …, 97, 99   (note that 98 is omitted).
 *
 * For a given index n (0‑based), let r = n mod 198.
 * The pair index is p = r / 2 (0 ≤ p ≤ 98).
 * The two‑digit number represented by the pair is:
 *     v = p      for p = 0 … 97
 *     v = 99     for p = 98
 * The digit at position n is:
 *     high digit  of v  if r is even   (i.e. n even)
 *     low digit   of v  if r is odd    (i.e. n odd)
 */
seq_t a(idx_t n)
{
    const idx_t PERIOD = 198ULL;          /* 99 two‑digit pairs × 2 */
    idx_t r = n % PERIOD;                /* position in the period */
    idx_t p = r / 2;                     /* pair index 0 … 98 */
    seq_t v = (p == 98ULL) ? (seq_t)99 : (seq_t)p;
    seq_t digit = (r & 1ULL) ? (v % 10) : (v / 10);
    return digit;
}
