/*
 * A080481  Concatenation of next n numbers (a(1) = 0).
 * https://oeis.org/A080481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenation of the consecutive integers from (n‑1)n/2
 *            up to (n‑1)n/2 + n – 1, with a(1) defined as 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed powers of 10 in seq_t type for fast access.   */
    static const seq_t pow10_list[11] = {
        /* 10^0 ... 10^10 */
        (seq_t)1ULL,
        (seq_t)10ULL,
        (seq_t)100ULL,
        (seq_t)1000ULL,
        (seq_t)10000ULL,
        (seq_t)100000ULL,
        (seq_t)1000000ULL,
        (seq_t)10000000ULL,
        (seq_t)100000000ULL,
        (seq_t)1000000000ULL,
        (seq_t)10000000000ULL
    };

    seq_t result = 0;                     /* accumulated concatenation   */

    /* Starting value of the block for index n (using idx_t arithmetic). */
    idx_t start = (n >= 1) ? ((n - 1) * n) / 2 : 0;

    for (idx_t i = 0; i < n; ++i) {
        idx_t num_val = start + i;              /* integer to append          */
        seq_t num = (seq_t)num_val;             /* cast for seq_t operations  */

        /* Compute decimal digit count of num_val. */
        int digits = 0;
        idx_t temp = num_val;
        do {
            ++digits;
            temp /= 10;
        } while (temp > 0);

        /* Append the integer by shifting and adding. */
        result = result * pow10_list[digits] + num;
    }

    return result;
}
