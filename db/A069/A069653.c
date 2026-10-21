/*
 * A069653  Smallest n-digit number with only prime divisors 2 or 3 (i.e., of
 *          the form 2^a * 3^b).
 * https://oeis.org/A069653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest integer of the form 2^a * 3^b that has at least
 *            n decimal digits (i.e. 2^a * 3^b ≥ 10^(n-1))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation searches over all exponents  a of 2
    until  2^a  exceeds the threshold.  For each a the
    minimal exponent  b  of 3  is found from a logarithmic
    estimate and then corrected by a small loop.  The
    exponentiation is done by binary powering, which works
    for built‑in integer types as well as for the arbitrary
    precision build.  The computed candidate is compared
    directly with the current best so no digit counting
    routine is required.  */

#include <math.h>

/* Auxiliary routine: binary powering for seq_t.  */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)   result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* 0 is outside the OEIS offset – return 0 for safety. */
    if (n == 0) return (seq_t)0;

    const long double LOG10_2 = log10l((long double)2.0L);
    const long double LOG10_3 = log10l((long double)3.0L);
    const long double TARGET_LOG10 = (long double)(n - 1);   /* log10(10^(n-1)) */

    /* 10^(n-1) – used as the digit threshold.  */
    const seq_t threshold = pow_seq_t((seq_t)10, n - 1);

    seq_t best = (seq_t)0;        /* placeholder for the minimal candidate */
    int found  = 0;                /* has a candidate been found yet?   */

    seq_t val2 = (seq_t)1;        /* 2^a  during the loop                */

    for (idx_t ea = 0; ; ++ea)
    {
        /* Estimate the minimal exponent of 3 needed.  */
        long double remaining = TARGET_LOG10 - ea * LOG10_2;
        idx_t b;
        if (remaining <= 0.0L)
            b = 0;
        else
            b = (idx_t)ceill(remaining / LOG10_3);

        /* Build the candidate 2^a * 3^b.  */
        seq_t candidate = val2 * pow_seq_t((seq_t)3, b);

        /* Correct a potential under‑estimate caused by rounding.  */
        while (candidate < threshold)
        {
            candidate *= (seq_t)3;
            ++b;
        }

        /* Update the best candidate.  */
        if (!found || candidate < best)
        {
            best  = candidate;
            found = 1;
        }

        /* Termination: once 2^a alone already reaches the
           threshold and no 3’s are needed, all larger a’s  
           will only increase the value.  */
        if (val2 >= threshold && b == 0)
            break;

        /* Advance to the next power of 2.  */
        val2 *= (seq_t)2;
    }

    return best;
}
