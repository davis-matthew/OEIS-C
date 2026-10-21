/*
 * A224997  Floor(1/f(x^(1/n))) for x = 17, where f computes the fractional
 *          part.
 * https://oeis.org/A224997
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/log(17)-1/2) for n>7; Special values for 2 ≤ n ≤
 *            7 are taken from the OEIS entry
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the initial values explicitly (offset = 2). */
    if (n <= 7) {
        /* a(2)=8, a(3)=1, a(4)=32, a(5)=1, a(6)=1, a(7)=2 */
        static const seq_t small[6] = {8, 1, 32, 1, 1, 2};
        return small[n - 2];
    }

    /* Rational approximation of 1/log(17). 1/log(17) ≈ 0.352956124 . */
    const unsigned long long NUM = 352956124ULL;   /* numerator   */
    const unsigned long long DEN = 1000000000ULL;   /* denominator */

    /* Compute floor(n * NUM / DEN - 0.5) without floating‑point. */
    unsigned long long prod = (unsigned long long)n * NUM;   /* n * NUM */
    unsigned long long int_part = prod / DEN;               /* floor(n*NUM/DEN) */
    unsigned long long rem = prod % DEN;                    /* remainder * 1/DEN */

    /* floor(x - 0.5) = int_part if fractional part >= 0.5, else int_part - 1 */
    if (rem * 2ULL < DEN)
        return (seq_t)(int_part - 1);
    else
        return (seq_t)int_part;
}
