/*
 * A046075  Nontrivial undulants; base 10 numbers >100 which are of the form
 *          aba, abab, ababa, ..., where a != b.
 * https://oeis.org/A046075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Construct the (n‑th) number consisting of alternating
 *            digits a and b (a != b), with length L = (n-1)/81 + 3. The pair
 *            (a,b) is obtained from n by indexing the 81 combinations
 *            (a=1..9, b=0..9 except a). The numbers are listed by increasing
 *            length and, within each length, by ascending a and...
 * Range:     u8 n <= 14, u16 n <= 212, u32 n <= 597, u64 n <= 1385,
 *            u128 n <= 2937, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine length L (each length contributes 81 numbers) */
    idx_t len = (n - 1) / 81 + 3;               /* length of the number */
    idx_t pos = (n - 1) % 81;                   /* position inside this length group */

    /* Determine digits a (1..9) and b (0..9, b != a) */
    idx_t a_digit = pos / 9 + 1;                /* a = 1..9 */
    idx_t r = pos % 9;                          /* 0..8 is the offset for b */
    seq_t b_digit = r < a_digit ? r : r + 1;    /* skip a when enumerating b */

    /* Build the number by iterating over its digits */
    seq_t value = 0;
    for (idx_t i = 0; i < len; ++i) {
        value = value * 10 + ((i % 2 == 0) ? a_digit : b_digit);
    }
    return value;
}
