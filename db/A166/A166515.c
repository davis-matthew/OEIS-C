/*
 * A166515  Partial sum of A166514.
 * https://oeis.org/A166515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 723, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived formula for A166515 (partial sums of A166514):
   a(n) = (2n² + 6n + 5 + (2n‑1)(‑1)^n + t(n)) / 16
   where t(n) =  4   if n mod 4 is 1 or 2
               -4   otherwise   (n mod 4 is 0 or 3)
   The expression is always an integer.                */

seq_t a(idx_t n)
{
    /* Work with signed 64‑bit intermediate to avoid underflow */
    long long nn = n;                       /* cast unsigned index to signed */
    long long num = 2 * nn * nn + 6 * nn + 5;

    /* Add or subtract (2n‑1) depending on parity of n */
    if ((n & 1ULL) == 0)                   /* n even -> add (2n‑1) */
        num += (2 * nn - 1);
    else                                   /* n odd  -> subtract (2n‑1) */
        num -= (2 * nn - 1);

    /* Apply the t(n) adjustment based on n mod 4 */
    switch (n % 4ULL) {
        case 1:
        case 2:
            num += 4;
            break;
        default:                          /* case 0 or 3 */
            num -= 4;
            break;
    }

    /* Divide by 16 to obtain the final sequence term */
    return (seq_t)(num / 16);
}
