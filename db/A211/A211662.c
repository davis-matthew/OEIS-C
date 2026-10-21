/*
 * A211662  Number of iterations log_3(log_3(log_3(...(n)...))) such that the
 *          result is < 2.
 * https://oeis.org/A211662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of times we can apply log base 3 to n before the
 *            value becomes < 2
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;    /* current value to be log‑ed */
    seq_t cnt = 0;         /* iteration counter */

    while (x >= 2) {
        /* compute floor(log_3(x)) */
        seq_t pow3 = 1;
        unsigned k = 0;
        while (pow3 <= x) {
            pow3 *= 3;
            ++k;
        }
        x = (seq_t)(k - 1);
        ++cnt;
    }
    return cnt;
}
