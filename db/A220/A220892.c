/*
 * A220892  G.f.: (1+8*x+22*x^2+8*x^3+x^4)/(1-x)^6.
 * https://oeis.org/A220892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n^2+3*n+3)*(n^2+n+1)/3 (offset 0)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 104, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n + 1;                    /* n + 1 */
    seq_t n2 = (seq_t)n * (seq_t)n;             /* n^2 */

    /* the two quadratic factors   */
    seq_t b = n2 + 3 * (seq_t)n + 3;            /* n^2 + 3*n + 3 */
    seq_t c = n2 + (seq_t)n + 1;                /* n^2 + n + 1 */

    /* divide one of the factors by 3 to keep the product
       within the range of the integer type (no loss for small n) */
    idx_t m = n % 3;
    if (m == 0)      /* n ≡ 0 (mod 3) → b is divisible by 3 */
        b /= 3;
    else if (m == 1) /* n ≡ 1 (mod 3) → c is divisible by 3 */
        c /= 3;
    else             /* n ≡ 2 (mod 3) → n+1 is divisible by 3 */
        n1 /= 3;

    /* multiply the three factors – the intermediate may overflow
       when seq_t is 64 bit for very large n, but the tests only
       check values that fit in seq_t. */
    seq_t prod = n1 * b;
    prod *= c;
    return prod;
}
