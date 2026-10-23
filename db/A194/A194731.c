/*
 * A194731  Digital roots of the nonzero octagonal numbers.
 * https://oeis.org/A194731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = digital root of the n-th nonzero octagonal number =
 *            A010888(A000567(n)) = ((n*(3*n-2)) % 9 == 0 ? 9 : (n*(3*n-2)) %
 *            9)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset 1, undefined for n < 1 */

    /* n mod 9 */
    idx_t rem = n % 9ULL;
    seq_t n_mod = (seq_t)rem;

    /* (3*n - 2) mod 9 */
    seq_t part = (3 * n_mod) % (seq_t)9;
    part = (part + (seq_t)9 - (seq_t)2) % (seq_t)9;

    /* (n mod 9) * ((3*n - 2) mod 9) mod 9 */
    seq_t prod = (n_mod * part) % (seq_t)9;

    return (prod == 0) ? (seq_t)9 : prod;
}
