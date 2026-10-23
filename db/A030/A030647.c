/*
 * A030647  Dimension of multiples of minimal representation of complex Lie
 *          algebra F4.
 * https://oeis.org/A030647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/1617000)*(2*n+11)*C(n+10,3)*C(n+3,3)*C(n+7,4)^2
 * Range:     u8 none, u16 none, u32 n <= 15, u64 n <= 87, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Binomial coefficient routine provided by the utilities */
#include "../utils/binomial.h"
/* GCD routine provided by the utilities */
#include "../utils/gcd.h"

/* Compute a(n) for the OEIS sequence A030647 */
seq_t a(idx_t n)
{
    /* Compute the binomial coefficients */
    seq_t t1 = binomial(n + 10, 3);      /* C(n+10,3) */
    seq_t t2 = binomial(n + 3, 3);       /* C(n+3,3)  */
    seq_t t3 = binomial(n + 7, 4);       /* C(n+7,4)  */

    /* Additional factor (2*n+11) */
    seq_t factor = (seq_t)2 * (seq_t)n + (seq_t)11;

    /* Denominator to cancel */
    idx_t  denom = 1617000;   /* 2^3 * 3 * 5^3 * 7^2 * 11 */

    seq_t result = 1;
    seq_t g;

    /* Helper macro to process each factor */
    #define PROCESS(x)                                        \
        do {                                                   \
            g = gcd((x), (seq_t)denom);                        \
            denom /= (idx_t)g;                                 \
            result *= (x) / g;                                 \
        } while (0)

    /* Multiply t3 twice, then the remaining factors */
    PROCESS(t3);
    PROCESS(t3);
    PROCESS(factor);
    PROCESS(t1);
    PROCESS(t2);

    /* Any remaining denominator (should be 1) */
    result *= (seq_t)denom;

    #undef PROCESS

    return result;
}
