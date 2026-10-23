/*
 * A028771  Nonsquares mod 58.
 * https://oeis.org/A028771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nonsquare modulo 58: least positive integer that is not a quadratic residue modulo 58 */
seq_t a(idx_t n)
{
    static int initialized = 0;
    static unsigned char is_qr[58];
    if (!initialized) {
        for (int i = 0; i < 58; ++i) is_qr[i] = 0;
        for (int y = 0; y < 58; ++y) {
            int r = (y * y) % 58;
            is_qr[r] = 1;
        }
        initialized = 1;
    }

    idx_t count = 0;
    for (idx_t i = 1; ; ++i) {
        unsigned r = (unsigned)(i % 58);
        if (!is_qr[r]) {
            ++count;
            if (count == n) {
                return (seq_t)i;
            }
        }
    }
}
