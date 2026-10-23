/*
 * A034054  Numbers with multiplicative digital root value 7.
 * https://oeis.org/A034054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers with exactly one digit 7 and any number of 1s, sorted ascending.
   The algorithm finds the digit length L such that L*(L+1)/2 >= n,
   then places the single 7 in the appropriate position within the L-digit number
   and builds the decimal representation. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    idx_t cumulative = 0;
    idx_t L = 0;
    /* Find smallest L with cumulative count >= n */
    while (cumulative < n) {
        ++L;
        cumulative += L;
    }
    idx_t prev = cumulative - L;     /* count before current length */
    idx_t k = n - prev - 1;          /* zero‑based index within length L */
    idx_t pos = L - 1 - k;           /* position of the digit 7 */

    seq_t result = 0;
    for (idx_t i = 0; i < L; ++i) {
        result = result * 10 + ((i == pos) ? 7 : 1);
    }
    return result;
}
