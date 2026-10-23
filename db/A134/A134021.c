/*
 * A134021  Length of n in balanced ternary representation.
 * https://oeis.org/A134021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceil(log(2*n+1)/log(3))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 by definition */

    idx_t m   = n;                  /* working copy of n   */
    idx_t len = 0;                  /* digit counter       */

    while (m > 0) {
        const idx_t r = m % 3;      /* current right‑most digit in base 3 */

        if (r == 2)                 /* digit 2 => -1 with a carry */
            m = (m + 1) / 3;
        else
            m =  m / 3;

        ++len;                      /* one more balanced digit      */
    }

    return (seq_t)len;              /* length fits comfortably in seq_t */
}
