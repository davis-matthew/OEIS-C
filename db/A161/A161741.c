/*
 * A161741  Fifth right hand column of the RSEG2 triangle A161739.
 * https://oeis.org/A161741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (n+5)*(n+4)*(n+3)*(n+2)*(n+1)*n * (175*n^6 + 1365*n^5 +
 *            2671*n^4 - 2805*n^3 - 14735*n^2 - 14445*n - 2466) / 5443200
 * Range:     i8 none, i16 none, i32 n <= 12, i64 n <= 88, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* offset is 1; values for n == 0 are undefined (caller will not request) */
    seq_t nn = (seq_t)n;
    seq_t result = 1;
    seq_t den = (seq_t)5443200;   /* 5443200 = 2^7 * 3^5 * 5^2 * 7 */

    /* multiply the six consecutive numbers, cancelling common factors with den */
    seq_t f;
    f = nn;     seq_t g = seq_gcd(f, den); if (g > 1) { f /= g; den /= g; } result *= f;
    f = nn + 1; g = seq_gcd(f, den); if (g > 1) { f /= g; den /= g; } result *= f;
    f = nn + 2; g = seq_gcd(f, den); if (g > 1) { f /= g; den /= g; } result *= f;
    f = nn + 3; g = seq_gcd(f, den); if (g > 1) { f /= g; den /= g; } result *= f;
    f = nn + 4; g = seq_gcd(f, den); if (g > 1) { f /= g; den /= g; } result *= f;
    f = nn + 5; g = seq_gcd(f, den); if (g > 1) { f /= g; den /= g; } result *= f;

    /* compute the polynomial */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;

    seq_t poly = 175*n6 + 1365*n5 + 2671*n4 - 2805*n3 - 14735*n2 - 14445*nn - 2466;

    /* cancel remaining factors with den */
    g = seq_gcd(poly, den);
    if (g > 1) { poly /= g; den /= g; }

    /* if any factor of den still remains, cancel it from result before final multiplication */
    g = seq_gcd(result, den);
    if (g > 1) { result /= g; den /= g; }

    /* final multiplication and division */
    result *= poly;
    if (den != 1) {
        result /= den;
    }

    return result;
}
