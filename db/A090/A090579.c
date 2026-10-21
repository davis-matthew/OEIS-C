/*
 * A090579  Number of numbers with 4 decimal digits and sum of digits = n.
 * https://oeis.org/A090579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts 4‑digit numbers whose digits sum to n.
   a(n) = |{ 1000 ≤ x ≤ 9999 : digitSum(x) = n }|
   Sum n ∈ [1,36]. For other n the result is 0. */
seq_t a(idx_t n)
{
    /* static array holding the counts for all possible sums */
    static seq_t counts[37] = {0};
    static int initialized = 0;

    if (!initialized) {
        /* Pre‑compute once.  There are 9000 four‑digit numbers. */
        for (int num = 1000; num <= 9999; ++num) {
            int sum = 0, m = num;
            while (m) {
                sum += m % 10;
                m /= 10;
            }
            counts[sum] = counts[sum] + 1;
        }
        initialized = 1;
    }

    /* n outside the valid range [1,36] yields no such numbers */
    if (n >= 1 && n <= 36)
        return counts[(int)n];
    return (seq_t)0;
}
