/*
 * A109614  n^3 followed by n followed by n^2.
 * https://oeis.org/A109614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = floor((n+2)/3)^((-3*(n mod 3)^2 + 5*(n mod 3) + 4)/2)
 * Range:     u8 n <= 18, u16 n <= 120, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* A109614: n^3 followed by n followed by n^2 */
    if (n == 0) return 0; /* undefined for offset 1, safe fallback */

    seq_t base = (seq_t)((n + 2) / 3);   /* floor((n+2)/3) */
    unsigned r = n % 3;                  /* n mod 3 */

    /* Exponent pattern: r=0 -> 2, r=1 -> 3, r=2 -> 1 */
    switch (r)
    {
        case 0:          /* exponent 2 */
            return base * base;
        case 1:          /* exponent 3 */
            return base * base * base;
        default:         /* r==2, exponent 1 */
            return base;
    }
}
