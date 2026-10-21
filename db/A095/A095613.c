/*
 * A095613  a(n) = 111 written in base 10 - n.
 * https://oeis.org/A095613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = integer obtained by writing 111 (decimal) in base (10‑n)
 *            and interpreting the digits as a decimal number. For n=9 (base
 *            1) this yields a string of 111 ones
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for 0 ≤ n ≤ 9. */
    if (n > 9) return 0;

    int base = (int)(10 - n);          /* base is positive for 0≤n≤9 */
    if (base == 1) {
        /* Unitary representation: 111 decimal becomes a string of 111 ones. */
        seq_t res = 0;
        for (idx_t i = 0; i < 111; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    seq_t temp = 111;          /* 111 in decimal */
    seq_t digits[32];          /* store digits LSB to MSB */
    int dlen = 0;

    while (temp > 0) {
        digits[dlen++] = temp % base;
        temp /= base;
    }

    seq_t res = 0;
    for (int i = dlen - 1; i >= 0; --i) {
        res = res * 10 + digits[i];
    }
    return res;
}
