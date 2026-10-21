/*
 * A074529  a(n) = 2^n + 3^n + 7^n.
 * https://oeis.org/A074529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 3^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fast binary exponentiation for each base */
    seq_t pow2 = 1;
    seq_t b = 2;
    idx_t e = n;
    while (e) {
        if (e & 1) pow2 *= b;
        e >>= 1;
        if (e) b *= b;
    }

    seq_t pow3 = 1;
    b = 3;
    e = n;
    while (e) {
        if (e & 1) pow3 *= b;
        e >>= 1;
        if (e) b *= b;
    }

    seq_t pow7 = 1;
    b = 7;
    e = n;
    while (e) {
        if (e & 1) pow7 *= b;
        e >>= 1;
        if (e) b *= b;
    }

    return pow2 + pow3 + pow7;
}
