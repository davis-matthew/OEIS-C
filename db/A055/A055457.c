/*
 * A055457  5^a(n) exactly divides 5n. Or, 5-adic valuation of 5n.
 * https://oeis.org/A055457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = v_5(5n) = v_5(n) + 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Protect against n = 0, which is not in the defined range */
    if (n == 0)
        return 0;

    seq_t exp = 0;          /* number of factors of 5 in n */
    idx_t m = n;
    while (m % 5 == 0) {
        exp++;
        m /= 5;
    }
    return exp + 1;         /* add the factor coming from the leading 5 in 5n */
}
