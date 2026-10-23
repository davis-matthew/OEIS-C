/*
 * A031469  Numbers whose base-4 representation has 4 fewer 0's than 3's.
 * https://oeis.org/A031469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest integer whose base‑4 representation has exactly
 *            four more 3's than 0's (OEIS A031469). The function computes
 *            this by brute‑force search
 * Range:     u8 n <= 1, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* sequence offset starts at 1 */
    seq_t count = 0;                          /* how many terms found so far */
    seq_t num    = 0;                          /* current candidate number   */
    seq_t target = (seq_t)n;                   /* n in terms of seq_t        */

    while (count < target) {
        ++num;                                 /* next candidate             */

        /* Count 0's and 3's in the base‑4 representation of num. */
        seq_t v = num;
        unsigned zeros = 0, threes = 0;
        while (v > 0) {
            unsigned digit = (unsigned)(v % 4);
            if (digit == 0)  ++zeros;
            else if (digit == 3) ++threes;
            v /= 4;
        }

        if (threes == zeros + 4) {
            ++count;                           /* valid term found          */
        }
    }
    return num;                                 /* the nth term              */
}
