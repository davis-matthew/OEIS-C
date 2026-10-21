/*
 * A094958  Numbers of the form 2^k or 5*2^k.
 * https://oeis.org/A094958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n=1), 2 (n=2), 4 (n=3), 5 (n=4), 2*a(n-2) for n≥5
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)5;

    /* For n ≥ 5 the recurrence a(n) = 2*a(n‑2) holds. */
    if (n & 1) {               /* odd indices → powers of 2   */
        unsigned exp = (n + 1) / 2;
        return ((seq_t)1) << exp;   /* 2^exp */
    } else {                    /* even indices → 5 * powers of 2 */
        unsigned exp = (n - 4) / 2;
        return ((seq_t)5) << exp;   /* 5 * 2^exp */
    }
}
