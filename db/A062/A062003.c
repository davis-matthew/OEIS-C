/*
 * A062003  Product of the k numbers formed by cyclically permuting digits of
 *          n (where k = number of digits of n).
 * https://oeis.org/A062003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = product of distinct integers obtained by cyclically
 *            permuting the decimal digits of n
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* decompose n into decimal digits */
    idx_t tmp = n;
    int digits[32];
    int len = 0;

    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        int rev[32];
        int revlen = 0;
        while (tmp > 0) {
            rev[revlen++] = tmp % 10;
            tmp /= 10;
        }
        for (int i = 0; i < revlen; ++i)
            digits[i] = rev[revlen - 1 - i];
        len = revlen;
    }

    /* store distinct rotations that have already been seen */
    seq_t seen[32];
    int seenlen = 0;

    seq_t result = 1;

    for (int r = 0; r < len; ++r) {
        /* build the rotation starting at position r */
        seq_t val = 0;
        for (int i = 0; i < len; ++i) {
            int d = digits[(r + i) % len];
            val = val * 10 + d;         /* use seq_t arithmetic */
        }

        /* skip duplicate rotations */
        int dup = 0;
        for (int j = 0; j < seenlen; ++j) {
            if (seen[j] == val) { dup = 1; break; }
        }
        if (!dup) {
            seen[seenlen++] = val;
            result *= val;               /* multiply into the product */
        }
    }

    return result;
}
