/*
 * A093783  Sum of digits of n in Roman numeral representation.
 * https://oeis.org/A093783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum of the numeric values of the Roman‑numeral symbols
 *            that form the standard representation of the integer n
 * Range:     u8 n <= 239, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Roman values and the sums contributed by each description. */
    static const int values[] = {
        1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1
    };
    static const int sums[] = {
        1000, 1100, 500, 600, 100, 110, 50, 60, 10, 11, 5, 6, 1
    };
    const int count = 13;          /* number of entries above */
    seq_t total = 0;

    /* work on a temporary copy of n, because we modify it below */
    for (idx_t rem = n; rem > 0; ) {
        /* find the largest value <= remaining number */
        for (int i = 0; i < count; ++i) {
            if (values[i] <= rem) {
                rem -= values[i];      /* consume that part of n   */
                total += sums[i];      /* add Roman digit values  */
                break;                 /* restart with new rem    */
            }
        }
    }

    return total;
}
