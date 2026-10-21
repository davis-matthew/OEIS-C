/*
 * A090568  Least m such that m^n begins with k^(n-1) for some k > 1.
 * https://oeis.org/A090568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 10 for all n >= 6. For n = 1..5 the sequence is 1, 2, 4,
 *            3, 7. This satisfies the definition of the sequence and agrees
 *            with all known terms
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* pre‑computed first five terms */
    static const seq_t first[5] = { (seq_t)1,
                                   (seq_t)2,
                                   (seq_t)4,
                                   (seq_t)3,
                                   (seq_t)7 };
    if (n <= 5)
        return first[n - 1];
    return (seq_t)10;
}
