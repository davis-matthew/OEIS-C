/*
 * A063017  a(n) = Catalan(n)*(3^(n+1) - 2^(n+1) + 1)/2.
 * https://oeis.org/A063017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Catalan(n)*(3^(n+1) - 2^(n+1) + 1)/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast exponentiation for seq_t --------------------------------------- */
/* Computes base^exp using O(log exp) multiplications.               */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Catalan number for index n ---------------------------------------- */
/* Uses the multiplicative formula:
 *   Catalan(n) = C(2n,n)/(n+1) =  prod_{i=1..n} (n+i)/i / (n+1)
 */
static seq_t catalan(idx_t n)
{
    seq_t res = 1;
    idx_t i;

    for (i = 1; i <= n; ++i)
    {
        /* res = res * (n + i) / i, all operations are exact integers */
        res = (res * ((seq_t)n + (seq_t)i)) / (seq_t)i;
    }

    /* divide by (n+1) to get the Catalan number */
    res = res / ((seq_t)n + (seq_t)1);
    return res;
}

/* Sequence a(n) ------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Catalan number */
    seq_t cat = catalan(n);

    /* Compute 3^(n+1) and 2^(n+1) */
    seq_t pow3 = pow_seq_t((seq_t)3, n + 1);
    seq_t pow2 = pow_seq_t((seq_t)2, n + 1);

    /* (3^(n+1) - 2^(n+1) + 1) */
    seq_t numerator = pow3 - pow2 + 1;

    /* (cat * numerator) / 2 */
    return (cat * numerator) / 2;
}
