/*
 * A170988  Carryless product n X n in base 7.
 * https://oeis.org/A170988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 342, u32 n <= 2400, u64 n <= 2400,
 *            u128 n <= 2400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Carryless product n X n in base 7 (OEIS A170988) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* decompose n into base‑7 digits (least‑significant first) */
    idx_t tmp = n;
    int digits[64];
    int len = 0;
    while (tmp != 0) {
        digits[len++] = tmp % 7;
        tmp /= 7;
    }
    /* multiply the digit polynomials modulo 7 (carryless product) */
    int prod[128] = {0};
    int maxdeg = 2 * len - 2;
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            prod[i + j] = (prod[i + j] + digits[i] * digits[j]) % 7;
        }
    }
    /* assemble the result as a base‑7 number */
    seq_t result = 0;
    for (int i = maxdeg; i >= 0; --i) {
        result = result * 7 + (seq_t)prod[i];
    }
    return result;
}
