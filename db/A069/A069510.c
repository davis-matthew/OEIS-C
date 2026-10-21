/*
 * A069510  a(1) = 1; a(n) = smallest palindrome of the form k*a(n-1) + 1.
 * https://oeis.org/A069510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest palindrome of the form k*a(n-1)+1.
 * For n ≤ 10 the terms are 1,2,3,4,5,6,7,8,9,55.
 * For n > 10 the closed form a(n) = (10^(n-8)-1)/9 (repunit).
 */
seq_t a(idx_t n)
{
    /* Handle the first ten terms explicitly. */
    if (n <= 10) {
        /* index 0 unused; base[1] = 1, ..., base[10] = 55. */
        static const seq_t base[11] = {
            0,          /* unused */
            1, 2, 3, 4, 5, 6, 7, 8, 9, 55
        };
        return base[n];
    }

    /* For n > 10 compute the repunit 1…1 of length (n-8). */
    seq_t val = 0;
    for (idx_t i = 0; i < n - 8; ++i) {
        val = val * 10 + 1;
    }
    return val;
}
