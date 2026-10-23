/*
 * A182287  If n = p*10^i + q*10^(i-1) + r*10^(i-2) + ... in decimal notation,
 *          then a(n) = p!*10^i + q!*10^(i-1) + r!*10^(i-2)+ ... .
 * https://oeis.org/A182287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = p!*10^i + q!*10^(i-1) + r!*10^(i-2)+ ... (base‑10 digits)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed factorials of 0..9 in seq_t type */
    static const seq_t fact[10] = {
        (seq_t)1,  /* 0! */
        (seq_t)1,  /* 1! */
        (seq_t)2,  /* 2! */
        (seq_t)6,  /* 3! */
        (seq_t)24, /* 4! */
        (seq_t)120,/* 5! */
        (seq_t)720,/* 6! */
        (seq_t)5040,/* 7! */
        (seq_t)40320,/* 8! */
        (seq_t)362880 /* 9! */
    };

    /* Special case for n = 0 (a(0) = 1 by definition) */
    if (n == 0)
        return (seq_t)1;

    idx_t tmp = n;
    seq_t result = 0;
    seq_t pow10   = 1;

    while (tmp > 0) {
        uint64_t digit = tmp % 10;               /* digit 0..9 */
        result += fact[digit] * pow10;           /* accumulate term   */
        pow10   *= 10;                           /* move to next order */
        tmp     /= 10;
    }

    return result;
}
