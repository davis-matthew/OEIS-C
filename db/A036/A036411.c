/*
 * A036411  9-gonal square numbers.
 * https://oeis.org/A036411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 898*a(n-2) - a(n-4) + 200 (n > 4) Initial values: a(1) =
 *            1 a(2) = 9 a(3) = 1089 a(4) = 8281 The sequence is defined only
 *            for n >= 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)1089;
    if (n == 4) return (seq_t)8281;

    seq_t prev4, prev2, next;

    /* Odd and even indices form two independent sequences. */
    if (n & 1) {          /* odd index */
        prev4 = (seq_t)1;          /* a(1)   */
        prev2 = (seq_t)1089;       /* a(3)   */
        for (idx_t i = 5; i <= n; i += 2) {
            next = ((seq_t)898 * prev2) - prev4 + (seq_t)200;
            prev4 = prev2;
            prev2 = next;
        }
    } else {             /* even index */
        prev4 = (seq_t)9;           /* a(2)   */
        prev2 = (seq_t)8281;        /* a(4)   */
        for (idx_t i = 6; i <= n; i += 2) {
            next = ((seq_t)898 * prev2) - prev4 + (seq_t)200;
            prev4 = prev2;
            prev2 = next;
        }
    }
    return prev2;
}
