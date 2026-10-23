/*
 * A028208  Expansion of 1/((1-6x)(1-7x)(1-10x)(1-12x)).
 * https://oeis.org/A028208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Series: Expansion of 1/((1-6x)(1-7x)(1-10x)(1-12x))
 * Recurrence: a(0)=1, a(1)=35
 *              a(n)=22*a(n-1)-120*a(n-2)+7^(n+1)-6^(n+1) , n>=2
 */
#include <stddef.h>

/* External integer types provided by the build system */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Handle the two initial terms directly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)35;

    /* Variables to hold the two previous terms */
    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)35;  /* a(n-1) */
    seq_t curr;               /* a(n) */

    /* Powers needed for the recurrence.
     * We keep 6^(i+1) and 7^(i+1).  Initially for i=2 we
     * need 6^3 and 7^3, which are obtained by multiplying the
     * starting values 6^2 and 7^2. */
    seq_t pow6 = (seq_t)36;   /* 6^2 */
    seq_t pow7 = (seq_t)49;   /* 7^2 */

    /* Iterate from i=2 up to the requested index n */
    for (idx_t i = 2u; i <= n; ++i)
    {
        /* Advance the powers: pow6 = 6^(i+1), pow7 = 7^(i+1) */
        pow6 = pow6 * (seq_t)6;
        pow7 = pow7 * (seq_t)7;

        /* Apply the recurrence */
        curr = ((seq_t)22 * prev1 - (seq_t)120 * prev2) + pow7 - pow6;

        /* Shift the window for the next iteration */
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
