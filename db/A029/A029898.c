/*
 * A029898  Pitoun's sequence: a(n+1) is digital root of a(0) + ... + a(n).
 * https://oeis.org/A029898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, nice, easy
 * Formula:   a(n) = digital root of 2^(n-1) in base 10, i.e. 2^(n-1) mod 9
 *            For n > 0, a(n+6) = a(n). Initial value a(0) = 1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* period of length 6 for 2^(k) mod 9 */
    idx_t r = (n - 1) % 6;
    static const seq_t tbl[6] = { (seq_t)1, (seq_t)2, (seq_t)4,
                                  (seq_t)8, (seq_t)7, (seq_t)5 };
    return tbl[r];
}
