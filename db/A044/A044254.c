/*
 * A044254  Numbers n such that string 0,3 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 81 * (9 * floor((n-1)/10) + (r < 9 ? (r + 1) : 9)) + (r <
 *            9 ? 3 : 27) where r = (n-1) mod 10. This enumerates all numbers
 *            n such that the base‑9 representation contains the substring
 *            "03" but n-1 does not
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out of bounds, not used */

    idx_t m = n - 1;                   /* zero‑based index */
    idx_t block = m / 10;              /* which group of 9 k’s */
    idx_t r = m % 10;                  /* position inside the block */
    idx_t k;
    seq_t val;

    if (r == 9) {                      /* the extra term for k divisible by 9 */
        k = 9 * block + 9;
        val = (seq_t)81 * k + 27;
    } else {                           /* the standard term 81*k + 3 */
        k = 9 * block + 1 + r;
        val = (seq_t)81 * k + 3;
    }

    return val;
}
