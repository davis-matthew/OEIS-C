/*
 * A129890  a(n) = (2*n+2)!! - (2*n+1)!!.
 * https://oeis.org/A129890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+2)!! - (2*n+1)!!
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t limit_even = 2 * n + 2;
    idx_t limit_odd  = 2 * n + 1;

    seq_t even = 1;
    for (idx_t k = 2; k <= limit_even; k += 2) {
        even *= k;
    }

    seq_t odd = 1;
    for (idx_t k = 1; k <= limit_odd; k += 2) {
        odd *= k;
    }

    return even - odd;
}
