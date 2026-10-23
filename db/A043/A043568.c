/*
 * A043568  Number of runs in base-16 representation of n.
 * https://oeis.org/A043568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of runs in the base-16 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n = 0: representation is "0" => one run */
    if (n == 0)
        return (seq_t)1;

    /* Count runs while extracting hexadecimal digits from least
       significant to most significant. */
    unsigned last_digit = (unsigned)(n & 0xF);         /* n % 16 */
    n >>= 4;                                          /* n /= 16 */
    unsigned runs = 1U;

    while (n != 0)
    {
        unsigned digit = (unsigned)(n & 0xF);         /* n % 16 */
        if (digit != last_digit)
            ++runs;
        last_digit = digit;
        n >>= 4;                                      /* n /= 16 */
    }
    return (seq_t)runs;
}
