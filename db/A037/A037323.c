/*
 * A037323  Numbers whose base-5 and base-7 expansions have the same digit
 *          sum.
 * https://oeis.org/A037323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th integer whose base‑5 and base‑7 digit sums are equal. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* Undefined for n=0 – return 0 */

    seq_t cand = 0;                       /* Current candidate number   */
    idx_t  count = 0;                     /* How many matches found     */

    while (count < n) {
        cand += 1;                        /* Test next integer */

        seq_t sum5 = 0, sum7 = 0;         /* Digit sums in base 5/7   */

        /* Digit sum in base 5 */
        seq_t t = cand;
        while (t) {
            sum5 += t % 5;
            t /= 5;
        }

        /* Digit sum in base 7 */
        t = cand;
        while (t) {
            sum7 += t % 7;
            t /= 7;
        }

        if (sum5 == sum7) ++count;        /* Found a match */
    }

    return cand;                          /* n-th term */
}
