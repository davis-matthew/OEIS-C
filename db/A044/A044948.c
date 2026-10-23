/*
 * A044948  Number of runs of odd length in the base 8 representation of n.
 * https://oeis.org/A044948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of runs of odd length in the base 8 representation of n */
seq_t a(idx_t n)
{
    /* Converting n to base 8 and counting odd‑length runs of equal digits. */
    unsigned char digits[64];          /* enough for any 64‑bit integer */
    int len = 0;
    idx_t v = n;

    /* Handle n == 0 explicitly (not part of the required offset) */
    if (v == 0) {          /* base‑8 representation is "0" – one run of length 1 */
        return 1;
    }

    /* Extract digits from least significant to most */
    do {
        digits[len++] = v % 8;
        v /= 8;
    } while (v);

    seq_t count = 0;
    int run_len = 1;
    unsigned char curr = digits[len - 1];

    for (int i = len - 2; i >= 0; --i) {
        unsigned char d = digits[i];
        if (d == curr) {
            ++run_len;
        } else {
            if ((run_len & 1) != 0) ++count;
            curr = d;
            run_len = 1;
        }
    }
    if ((run_len & 1) != 0) ++count;   /* final run */

    return count;
}
