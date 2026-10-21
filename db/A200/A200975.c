/*
 * A200975  Numbers on the diagonals in Ulam's spiral.
 * https://oeis.org/A200975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4k+1)=4k^2+4k+1, a(4k+2)=4k^2+6k+3, a(4k+3)=4k^2+8k+5,
 *            a(4k+4)=4k^2+10k+7
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert 1‑based index to 0‑based offset. */
    idx_t m = (n - 1) / 4;          /* k = floor((n-1)/4) */
    idx_t r = (n - 1) % 4;          /* remainder 0..3 */

    seq_t k = (seq_t)m;
    seq_t result;

    switch (r) {
        case 0: /* a(4k+1) */
            result = (seq_t)4 * k * k + (seq_t)4 * k + (seq_t)1;
            break;
        case 1: /* a(4k+2) */
            result = (seq_t)4 * k * k + (seq_t)6 * k + (seq_t)3;
            break;
        case 2: /* a(4k+3) */
            result = (seq_t)4 * k * k + (seq_t)8 * k + (seq_t)5;
            break;
        case 3: /* a(4k+4) */
            result = (seq_t)4 * k * k + (seq_t)10 * k + (seq_t)7;
            break;
        default:  /* impossible, but keep compiler happy */
            result = (seq_t)0;
            break;
    }
    return result;
}
