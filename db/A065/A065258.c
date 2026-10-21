/*
 * A065258  Quintal Queens permutation of N: halve or multiply by 3 (mod 5)
 *          each digit (0->0, 1->3, 2->1, 3->4, 4->2) of the base 5
 *          representation of n-1, add one.
 * https://oeis.org/A065258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 + f(n-1), where f is obtained by taking the base‑5
 *            digits of n-1, replacing each digit d by (3*d mod 5) via the
 *            mapping {0→0, 1→3, 2→1, 3→4, 4→2}, and interpreting the
 *            transformed digits as a base‑5 number
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* safety for n = 0 */
    static const unsigned mapping[5] = {0, 3, 1, 4, 2};
    idx_t m = n - 1;                           /* base-5 number to transform */
    seq_t result = 0;
    seq_t power = 1;                           /* current 5^k multiplier */
    while (m > 0) {
        int d = (int)(m % 5);                  /* least significant base‑5 digit */
        seq_t transformed = (seq_t)mapping[d];
        result += transformed * power;
        power *= (seq_t)5;
        m /= 5;
    }
    return result + (seq_t)1;
}
