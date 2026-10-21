/*
 * A095607  a(n) = 108 written in base 13 - n.
 * https://oeis.org/A095607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal value of the digits of 108 expressed in base
 *            (13‑n). For base‑1 this is the unary representation (108 ones)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Defined for 0 ≤ n ≤ 12.  For larger n return 0. */
    if (n > 12) return 0;

    unsigned int base = 13u - (unsigned int)n;   /* 1 ≤ base ≤ 13 */

    /* Unary base (base == 1) – 108 ones */
    if (base == 1) {
        seq_t res = 0;
        for (unsigned int i = 0; i < 108; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* Convert 108 to the given base and concatenate the digits as decimal. */
    const unsigned int value = 108;
    unsigned int rem = value;
    unsigned int digits[10];   /* 108 in base 2 has at most 7 digits. */
    int dcnt = 0;

    while (rem > 0) {
        digits[dcnt++] = rem % base;
        rem /= base;
    }

    seq_t res = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        res = res * 10 + digits[i];
    }
    return res;
}
