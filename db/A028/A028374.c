/*
 * A028374  Numbers that have only curved digits {0, 3, 6, 8, 9} or digits
 *          that are both curved and linear {2, 5}.
 * https://oeis.org/A028374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number obtained by interpreting (n‑1) in base‑7 * and
 *            replacing each base‑7 digit d by the allowed digit *
 *            {0,2,3,5,6,8,9} (mapping d↔[0,2,3,5,6,8,9])
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    idx_t tmp = n - 1;
    if (tmp == 0) return (seq_t)0;
    static const unsigned char mapping[7] = {0, 2, 3, 5, 6, 8, 9};
    unsigned char digits[64];
    int len = 0;
    while (tmp > 0) {
        digits[len++] = tmp % 7;
        tmp /= 7;
    }
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        res = res * 10 + mapping[digits[i]];
    }
    return res;
}
