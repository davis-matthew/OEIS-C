/*
 * A034266  Partial sums of A027818.
 * https://oeis.org/A034266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (7*n+1)*binomial(n+6, 7)/8
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 44, u64 n <= 752, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t prod = 1;
    /* Denominator 40320 = 2^7 * 3^2 * 5 * 7 */
    int c2 = 7, c3 = 2, c5 = 1, c7 = 1;
    seq_t f;

    /* (7*n+1) */
    f = (seq_t)7 * (seq_t)n + 1;
    while (c2 && (f % 2 == 0)) { f /= 2; c2--; }
    while (c3 && (f % 3 == 0)) { f /= 3; c3--; }
    while (c5 && (f % 5 == 0)) { f /= 5; c5--; }
    while (c7 && (f % 7 == 0)) { f /= 7; c7--; }
    prod *= f;

    /* n */
    f = (seq_t)n;
    while (c2 && (f % 2 == 0)) { f /= 2; c2--; }
    while (c3 && (f % 3 == 0)) { f /= 3; c3--; }
    while (c5 && (f % 5 == 0)) { f /= 5; c5--; }
    while (c7 && (f % 7 == 0)) { f /= 7; c7--; }
    prod *= f;

    /* n+1 */
    f = (seq_t)n + 1;
    while (c2 && (f % 2 == 0)) { f /= 2; c2--; }
    while (c3 && (f % 3 == 0)) { f /= 3; c3--; }
    while (c5 && (f % 5 == 0)) { f /= 5; c5--; }
    while (c7 && (f % 7 == 0)) { f /= 7; c7--; }
    prod *= f;

    /* n+2 */
    f = (seq_t)n + 2;
    while (c2 && (f % 2 == 0)) { f /= 2; c2--; }
    while (c3 && (f % 3 == 0)) { f /= 3; c3--; }
    while (c5 && (f % 5 == 0)) { f /= 5; c5--; }
    while (c7 && (f % 7 == 0)) { f /= 7; c7--; }
    prod *= f;

    /* n+3 */
    f = (seq_t)n + 3;
    while (c2 && (f % 2 == 0)) { f /= 2; c2--; }
    while (c3 && (f % 3 == 0)) { f /= 3; c3--; }
    while (c5 && (f % 5 == 0)) { f /= 5; c5--; }
    while (c7 && (f % 7 == 0)) { f /= 7; c7--; }
    prod *= f;

    /* n+4 */
    f = (seq_t)n + 4;
    while (c2 && (f % 2 == 0)) { f /= 2; c2--; }
    while (c3 && (f % 3 == 0)) { f /= 3; c3--; }
    while (c5 && (f % 5 == 0)) { f /= 5; c5--; }
    while (c7 && (f % 7 == 0)) { f /= 7; c7--; }
    prod *= f;

    /* n+5 */
    f = (seq_t)n + 5;
    while (c2 && (f % 2 == 0)) { f /= 2; c2--; }
    while (c3 && (f % 3 == 0)) { f /= 3; c3--; }
    while (c5 && (f % 5 == 0)) { f /= 5; c5--; }
    while (c7 && (f % 7 == 0)) { f /= 7; c7--; }
    prod *= f;

    /* n+6 */
    f = (seq_t)n + 6;
    while (c2 && (f % 2 == 0)) { f /= 2; c2--; }
    while (c3 && (f % 3 == 0)) { f /= 3; c3--; }
    while (c5 && (f % 5 == 0)) { f /= 5; c5--; }
    while (c7 && (f % 7 == 0)) { f /= 7; c7--; }
    prod *= f;

    /* All denominator factors should have been cancelled. */
    return prod;
}
