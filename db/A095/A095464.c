/*
 * A095464  a(n) = 36 written in base n.
 * https://oeis.org/A095464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the base‑n representation of 36 written as a decimal
 *            integer. For n = 1 the "base‑1" representation is 36 ones. For n
 *            ≥ 2 obtain the digits of 36 in base n by repeated division by n,
 *            then form the decimal integer by concatenating those digits
 *            (digits < 10 for all n used in the OEIS data)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 12, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)         /* offset is 1; n=0 is not used, but guard anyway */
        return 0;

    /* base‑1: 36 ones */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 36; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* base‑n, n ≥ 2 */
    uint64_t q = 36;          /* number to convert */
    int digits[16];           /* 36 in any base ≤ 36 needs ≤ 4 digits; buffer is safe */
    int len = 0;

    while (q > 0) {
        digits[len++] = q % n;
        q /= n;
    }

    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        res = res * 10 + digits[i];
    }
    return res;
}
