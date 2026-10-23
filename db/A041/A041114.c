/*
 * A041114  Numerators of continued fraction convergents to sqrt(66).
 * https://oeis.org/A041114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041114: Numerators of continued fraction convergents to sqrt(66)
   Formula: a(0)=8, a(1)=65.
   For n >= 2:
        if n is even:  a(n) = 16*a(n-1) + a(n-2)
        if n is odd :  a(n) =  8*a(n-1) + a(n-2)
   This uses only integer arithmetic and works for any idx_t and seq_t
   provided by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)8;
    if (n == 1) return (seq_t)65;

    seq_t prev2 = (seq_t)8;   /* a(0) */
    seq_t prev1 = (seq_t)65;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        if ((i & (idx_t)1) == 0)
            curr = 16 * prev1 + prev2;   /* even index */
        else
            curr = 8 * prev1 + prev2;    /* odd index */

        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
