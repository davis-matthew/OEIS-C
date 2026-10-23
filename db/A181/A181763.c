/*
 * A181763  a(n) = A061037(n)^2.
 * https://oeis.org/A181763
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A061037(n)^2 where A061037(n) = n^2 - 4, if n is odd;
 *            (n^2 - 4) / 4, if n ≡ 0 (mod 4); (n^2 - 4) / 16, if n ≡ 2 (mod
 *            4)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2; for smaller n the value is
       unspecified, but we return 0 for safety. */
    if (n < 2)
        return (seq_t)0;

    /* Compute n^2 - 4. */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t sub  = n_sq - 4;          /* n^2 - 4 */

    seq_t val;
    if ((n & 1U) == 1U) {           /* n is odd */
        val = sub;                  /* no division needed */
    } else {
        /* n is even.  Determine whether n/2 is even or odd. */
        if ((((n >> 1) & 1U) == 0U)) { /* n divisible by 4 */
            val = sub / 4;
        } else {                    /* n ≡ 2 (mod 4) */
            val = sub / 16;
        }
    }

    /* Return the square of A061037(n). */
    return val * val;
}
