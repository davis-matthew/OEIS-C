/*
 * A051513  a(n) = min { 2^n, floor(100/n) }.
 * https://oeis.org/A051513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min( 2^n , floor(100 / n) ) offset 1
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; sequence is defined for n ≥ 1 */
    seq_t floor_val = (seq_t)(100 / n);

    if (n <= 4) {                  /* 2^n is small enough to compute safely */
        seq_t pow_val = ((seq_t)1) << n;
        return (pow_val < floor_val) ? pow_val : floor_val;
    }
    return floor_val;              /* for n ≥ 5, 2^n > floor(100/n) */
}
