/*
 * A133486  a(n) = Sum_{ k = 0 to n-1} ( subtract k modulo 9 from 9, multiply
 *          this by k-th power of 10 ).
 * https://oeis.org/A133486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0}^{n-1} ((9-(k % 9))*10^k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe guard – n is ≥1 by definition */

    seq_t result = 0;              /* accumulates the sum */
    seq_t pow10  = 1;              /* 10^k, starts with 10^0 = 1 */
    seq_t digit  = 9;              /* least‑significant digit for k=0 */

    for (idx_t i = 0; i < n; ++i) {
        result += digit * pow10;   /* add (9-(k%9)) * 10^k */

        pow10 *= 10;              /* advance 10^k  → 10^(k+1) */

        /* rotate digit: 9 8 7 6 5 4 3 2 1 9 8 ...  */
        if (digit == 1)
            digit = 9;
        else
            --digit;
    }

    return result;
}
