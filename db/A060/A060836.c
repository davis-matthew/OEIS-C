/*
 * A060836  Number of permutations of n letters where exactly 5 change
 *          position.
 * https://oeis.org/A060836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 44*binomial(n,5), a(n)=0 for n<5
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 105, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes gcd for seq_t values (Euclidean algorithm). */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* For n < 5 the value is 0. */
    if (n < (idx_t)5) {
        return (seq_t)0;
    }

    const idx_t k = (idx_t)5;
    seq_t result = (seq_t)1;

    /* Incrementally compute binomial(n,5) as product of numerators
       divided by the denominators, reducing by gcd at each step to
       keep intermediate values small. */
    for (idx_t i = (idx_t)0; i < k; ++i) {
        seq_t num = (seq_t)(n - k + (idx_t)1 + i);   /* n-4, n-3, ..., n */
        seq_t den = (seq_t)(i + (idx_t)1);          /* 1,2,3,4,5 */

        seq_t g = seq_gcd(num, den);
        if (g > (seq_t)1) {
            num /= g;
            den /= g;
        }

        result *= num;
        /* Since den divides result*unit after the reduction, the division
           is exact. */
        result /= den;
    }

    /* Multiply by 44 to obtain the final term. */
    result *= (seq_t)44;
    return result;
}
