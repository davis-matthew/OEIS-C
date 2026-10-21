/*
 * A082741  Numbers that have digits consisting only of line segments or both
 *          line segments and curves (base 10 digits are 1, 2, 4, 5, 7).
 * https://oeis.org/A082741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = the nth smallest positive integer whose decimal digits
 *            are restricted to the digit set {1,2,4,5,7}
 * Range:     u8 n <= 74, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* mapping from base‑5 digit to allowed decimal digit */
    static const seq_t map[5] = {(seq_t)1, (seq_t)2, (seq_t)4, (seq_t)5, (seq_t)7};

    /* find the length L of the number (how many digits) */
    idx_t c   = 0;      /* count of numbers with fewer digits */
    idx_t pow = (idx_t)5;  /* 5^1 */
    idx_t L   = 1;       /* current length */

    while (c + pow < n) {
        c  += pow;
        pow *= (idx_t)5;
        ++L;
    }

    /* index of the desired number within the block of length L (0‑based) */
    idx_t r = n - c - 1;

    /* store base‑5 digits; L is guaranteed to be small (≤ 64 for n up to 10⁵) */
    int digits[64];
    for (idx_t i = 0; i < L; ++i) {
        digits[L - 1 - i] = (int)(r % (idx_t)5);
        r /= (idx_t)5;
    }

    /* build the result in decimal by mapping each digit */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < L; ++i) {
        result = result * (seq_t)10 + map[digits[i]];
    }

    return result;
}
