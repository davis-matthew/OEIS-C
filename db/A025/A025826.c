/*
 * A025826  Expansion of 1/((1-x^2)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A025826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 23*n + 11*(n+9)*(-1)^n + 356 + * 9*(2*((n+2)
 *            mod 10) - 2*(n mod 10) + ((n+4) mod 10) - ((n+8) mod 10)))/440)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    long long nn = (long long)n;

    /* n^2 and 23*n */
    long long part1 = nn * nn;      /* n^2 */
    long long part2 = 23 * nn;      /* 23*n */

    /* 11*(n+9)*(-1)^n  */
    long long part3 = 11 * (nn + 9);
    if (n & 1LL) {                 /* odd n => multiply by -1 */
        part3 = -part3;
    }

    /* 9*(2*((n+2) mod 10) - 2*(n mod 10) + ((n+4) mod 10) - ((n+8) mod 10)) */
    long long m0 = nn % 10;
    long long m2 = (nn + 2) % 10;
    long long m4 = (nn + 4) % 10;
    long long m8 = (nn + 8) % 10;
    long long mod_part = 9 * (2 * m2 - 2 * m0 + m4 - m8);

    long long numerator = part1 + part2 + 356 + mod_part + part3;

    /* floor division by 440 */
    long long result = numerator / 440;

    return (seq_t)result;
}
