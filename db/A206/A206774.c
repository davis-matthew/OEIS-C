/*
 * A206774  First differences of A033922.
 * https://oeis.org/A206774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   For n ≥ 1 - a(2n+1) = 1 - a(4n+2) = 0 - a(4*A042968(n)) = −1
 *            (indices 4, 8, 12, 20, 24, …) - a(16*A042968(n)) = −4 (indices
 *            16, 32, 48, 80, 96, …) - a(64*(2n+1)) = −6 (indices 64, 192,
 *            320, 448, …) Otherwise a(n) = 0 (including the special case n =
 *            1 → 0). A042968(n) denotes the positive integers...
 * Range:     i8 n <= 9964, i16 n <= 9964, i32 n <= 9964, i64 n <= 9964,
 *            i128 n <= 9964, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;                 /* special first term */

    /* odd index (except 1) */
    if (n & 1) return 1;

    /* even index 2 (mod 4) */
    if ((n & 3) == 2) return 0;

    /* now n is a multiple of 4 */
    /* check for 64*(2k+1) = 64, 192, 320, … */
    if (n % 64 == 0) {
        idx_t t = n / 64;
        if (t & 1)          /* t odd → n = 64*(2k+1) */
            return (seq_t)-6;
        /* t even → index divisible by 128, default to 0 */
        return 0;
    }

    /* check for 16*A042968(m) (m not divisible by 4) */
    if (n % 16 == 0) {
        idx_t m = n / 16;
        if ((m & 3) != 0)   /* m mod 4 ≠ 0 */
            return (seq_t)-4;
    }

    /* remaining indices: multiples of 4 but not multiples of 16,
       i.e., indices 4,8,12,20,24,… → −1 */
    return (seq_t)-1;
}
