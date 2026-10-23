/*
 * A026124  a(n) = number of (s(0),s(1),...,s(n)) such that every s(i) is a
 *          nonnegative integer, s(0) = 1, s(n) = 3, |s(1) - s(0)| = 1, |s(i)
 *          - s(i-1)| <= 1 for i >= 2. Also a(n) = T(n,n-2), where T is the
 *          array in A026120.
 * https://oeis.org/A026124
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((5*n+19)*a(n-1) - 4*n*a(n-2) - 8*(n+1)*a(n-3) +
 *            (5*n-22)*a(n-4) + 3*(n-5)*a(n-5)) / (n+6) Initial values:
 *            a(2)=1, a(3)=2, a(4)=7, a(5)=20, a(6)=59
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return early for the known initial terms */
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)7;
    if (n == 5) return (seq_t)20;
    if (n == 6) return (seq_t)59;

    /* Allocate array for values up to n (index 0..n) */
    seq_t *val = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!val) return (seq_t)0; /* allocation failure – unlikely */

    /* Initialize stored values */
    val[2] = (seq_t)1;
    val[3] = (seq_t)2;
    val[4] = (seq_t)7;
    val[5] = (seq_t)20;
    val[6] = (seq_t)59;

    /* Compute a(n) for n >= 7 using the recurrence */
    for (idx_t i = 7; i <= n; ++i)
    {
        seq_t s1 = val[i - 1];
        seq_t s2 = val[i - 2];
        seq_t s3 = val[i - 3];
        seq_t s4 = val[i - 4];
        seq_t s5 = val[i - 5];

        seq_t term1 = ((seq_t)5 * (seq_t)i + (seq_t)19) * s1;
        seq_t term2 = ((seq_t)4 * (seq_t)i) * s2;
        seq_t term3 = ((seq_t)8 * ((seq_t)i + (seq_t)1)) * s3;
        seq_t term4 = ((seq_t)5 * (seq_t)i - (seq_t)22) * s4;
        seq_t term5 = ((seq_t)3 * ((seq_t)i - (seq_t)5)) * s5;

        /* positive contributions */
        seq_t pos = term1 + term4 + term5;
        /* negative contributions */
        seq_t neg = term2 + term3;

        /* the numerator is positive - negative */
        seq_t numer = pos - neg;

        val[i] = numer / ((seq_t)i + (seq_t)6);
    }

    seq_t result = val[n];
    free(val);
    return result;
}
