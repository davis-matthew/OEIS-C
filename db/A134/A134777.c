/*
 * A134777  First digit of n alphabetically.
 * https://oeis.org/A134777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* First alphabetically ordered decimal digit of n
   Alphabetical order of digit names (English):
   8 < 5 < 4 < 9 < 1 < 7 < 6 < 3 < 2 < 0 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* a(0) = 0 */

    /* rank[d] = position of digit d in the alphabetical order listed above */
    static const int rank[10] = {9, 4, 8, 7, 2, 1, 6, 5, 0, 3};

    int best_digit = 9;            /* = 9, will be replaced by the first seen digit */
    int best_rank  = 10;           /* higher than any possible rank */

    while (n > 0) {
        int d = (int)(n % 10);          /* current least significant decimal digit */
        int r = rank[d];
        if (r < best_rank) {
            best_rank  = r;
            best_digit = d;
        }
        n /= 10;
    }
    return (seq_t)best_digit;
}
