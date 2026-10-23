/*
 * A131669  Odd digits followed by positive even digits.
 * https://oeis.org/A131669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = cycle[(n-1) mod 9] where cycle = {1,3,5,7,9,2,4,6,8}
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t cycle[9] = {
        (seq_t)1, (seq_t)3, (seq_t)5, (seq_t)7, (seq_t)9,
        (seq_t)2, (seq_t)4, (seq_t)6, (seq_t)8
    };
    idx_t r = (n - 1) % 9;   /* 0‑based remainder of the cycle */
    return cycle[r];
}
