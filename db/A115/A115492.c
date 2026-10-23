/*
 * A115492  Number of monic irreducible polynomials of degree 2 in
 *          GF(2^n)[x,y].
 * https://oeis.org/A115492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 54*a(n-1)-808*a(n-2)+3456*a(n-3)-4096*a(n-4)
   with a(1)=35, a(2)=1134, a(3)=34748, a(4)=1081080.  */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1. */
    if (n < 1)
        return (seq_t)0;

    /* Base cases */
    switch (n)
    {
        case 1: return (seq_t)35;
        case 2: return (seq_t)1134;
        case 3: return (seq_t)34748;
        case 4: return (seq_t)1081080;
    }

    /* Recurrence variables: prev4=a(n-4), prev3=a(n-3), prev2=a(n-2), prev1=a(n-1) */
    seq_t prev4 = (seq_t)35;
    seq_t prev3 = (seq_t)1134;
    seq_t prev2 = (seq_t)34748;
    seq_t prev1 = (seq_t)1081080;
    seq_t cur   = 0;

    for (idx_t i = 5; i <= n; ++i)
    {
        cur = ((seq_t)54  * prev1)
            + ((seq_t)3456 * prev3)
            - ((seq_t)808  * prev2)
            - ((seq_t)4096 * prev4);

        /* shift window for next iteration */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
