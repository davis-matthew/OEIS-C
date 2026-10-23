/*
 * A126760  a(0) = 0, a(2n) = a(n), a(3n) = a(n), a(6n+1) = 2n + 1, a(6n+5) =
 *          2n + 2.
 * https://oeis.org/A126760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 765, u16 n <= 9460, u32 n <= 9460, u64 n <= 9460,
 *            u128 n <= 9460, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A126760: 
 * a(0) = 0
 * a(2n) = a(n)
 * a(3n) = a(n)
 * a(6n+1) = 2n + 1
 * a(6n+5) = 2n + 2
 *
 * The implementation follows the recurrence directly, iteratively reducing
 * the index until a terminating case is reached.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t m = n;

    while (m > 0) {
        idx_t r = m % 6;

        if (r == 1) {
            result = ((seq_t)2 * ((seq_t)(m - 1) / 6)) + 1;
            break;
        }
        if (r == 5) {
            result = ((seq_t)2 * ((seq_t)(m - 5) / 6)) + 2;
            break;
        }
        if ((m & 1) == 0) {          /* divisible by 2 */
            m >>= 1;
            continue;
        }
        if (m % 3 == 0) {             /* divisible by 3 */
            m /= 3;
            continue;
        }
        /* In theory this branch should never be reached because any
           positive integer is either 0 mod 2, 3, 1 or 5 mod 6. */
        break;
    }

    /* If m became 0 (only when n==0), result stays 0. */
    return result;
}
