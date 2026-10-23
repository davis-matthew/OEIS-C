/*
 * A018608  Divisors of 716.
 * https://oeis.org/A018608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 716 sorted in increasing order */
/* Terms: 1, 2, 4, 179, 358, 716 */

seq_t a(idx_t n)
{
    /* static array containing all divisors of 716 in ascending order */
    static const seq_t divisors[6] = {1, 2, 4, 179, 358, 716};

    if (n == 0)      /* below the offset – undefined, return 0 */
        return (seq_t)0;

    if (n <= 6) {
        unsigned long long idx = n - 1;   /* safe conversion for array index */
        return divisors[idx];
    }

    /* for any n > 6, the sequence has no defined value */
    return (seq_t)0;
}
