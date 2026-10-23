/*
 * A108650  a(n) = (n+1)^2*(n+2)*(n+3)*(3*n+4)/24.
 * https://oeis.org/A108650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^2*(n+2)*(n+3)*(3*n+4)/24
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 126, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Convert index to seq_t first to avoid intermediate overflow. */
    seq_t f1 = (seq_t)n + 1;                 /* (n+1)             */
    seq_t f2 = f1;                           /* (n+1)             */
    seq_t f3 = (seq_t)n + 2;                 /* (n+2)             */
    seq_t f4 = (seq_t)n + 3;                 /* (n+3)             */
    seq_t f5 = ((seq_t)3 * (seq_t)n) + 4;    /* (3*n+4)           */

    seq_t result = 1;
    seq_t den     = 24;                       /* Denominator       */

    /* Cancel common factors with the denominator to keep intermediates small. */
    {
        seq_t g = seq_gcd(f1, den);
        if (g != 1) { f1 /= g; den /= g; }
        result *= f1;
    }
    {
        seq_t g = seq_gcd(f2, den);
        if (g != 1) { f2 /= g; den /= g; }
        result *= f2;
    }
    {
        seq_t g = seq_gcd(f3, den);
        if (g != 1) { f3 /= g; den /= g; }
        result *= f3;
    }
    {
        seq_t g = seq_gcd(f4, den);
        if (g != 1) { f4 /= g; den /= g; }
        result *= f4;
    }
    {
        seq_t g = seq_gcd(f5, den);
        if (g != 1) { f5 /= g; den /= g; }
        result *= f5;
    }

    /* If any part of the denominator remains, divide once more. */
    if (den != 1)
    {
        result /= den;
    }

    return result;
}
