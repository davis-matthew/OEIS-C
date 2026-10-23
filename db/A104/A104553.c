/*
 * A104553  Sum of trapezoid weights of all Schroeder paths of length 2n.
 * https://oeis.org/A104553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by the recurrence n*(2*n-3)*a(n) =
 *            2*(8*n^2-15*n+5)*a(n-1) - 2*(14*n^2-28*n+11)*a(n-2) +
 *            2*(8*n^2-17*n+7)*a(n-3) - (n-2)*(2*n-1)*a(n-4) Initial values:
 *            a(1)=1, a(2)=7, a(3)=38, a(4)=198, a(5)=1039
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;        /* memoized values, index 0 unused */
static idx_t built = 0;           /* highest index already computed */

/* helper for the first 5 values */
static const seq_t init[6] = {
    (seq_t)0,          /* dummy for index 0 */
    (seq_t)1,          /* a(1) */
    (seq_t)7,          /* a(2) */
    (seq_t)38,         /* a(3) */
    (seq_t)198,        /* a(4) */
    (seq_t)1039       /* a(5) */
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n<1 */

    if (n > built) {
        /* grow the memo array */
        seq_t *tmp = (seq_t *)realloc(memo, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failed */
        memo = tmp;
        memo[0] = (seq_t)0;               /* dummy */

#if !defined(__cplusplus)
        /* C implementation – use 128‑bit intermediate for 64‑bit seq_t */
        typedef unsigned __int128 u128;
        for (idx_t i = built + 1; i <= n; ++i) {
            if (i <= 5) {
                memo[i] = init[i];
                continue;
            }
            u128 ns = (unsigned long long)i;

            u128 lhs = ns * ((unsigned long long)2 * ns - (unsigned long long)3);

            u128 termA = (unsigned long long)2 *
                         ((unsigned long long)8 * ns * ns -
                          (unsigned long long)15 * ns +
                          (unsigned long long)5) *
                         (unsigned long long)memo[i - 1];

            u128 termB = (unsigned long long)2 *
                         ((unsigned long long)14 * ns * ns -
                          (unsigned long long)28 * ns +
                          (unsigned long long)11) *
                         (unsigned long long)memo[i - 2];

            u128 termC = (unsigned long long)2 *
                         ((unsigned long long)8 * ns * ns -
                          (unsigned long long)17 * ns +
                          (unsigned long long)7) *
                         (unsigned long long)memo[i - 3];

            u128 termD = (ns - (unsigned long long)2) *
                         ((unsigned long long)2 * ns - (unsigned long long)1) *
                         (unsigned long long)memo[i - 4];

            u128 rhs = termA + termC - termB - termD;

            memo[i] = (seq_t)(rhs / lhs);
        }
#else
        /* C++ implementation – seq_t may be an arbitrary‑precision class;
           rely solely on seq_t arithmetic. */
        for (idx_t i = built + 1; i <= n; ++i) {
            if (i <= 5) {
                memo[i] = init[i];
                continue;
            }
            seq_t ns = (seq_t)i;

            seq_t lhs = ns * ((seq_t)2 * ns - (seq_t)3);

            seq_t termA = (seq_t)2 *
                          ((seq_t)8 * ns * ns - (seq_t)15 * ns + (seq_t)5) *
                          memo[i - 1];

            seq_t termB = (seq_t)2 *
                          ((seq_t)14 * ns * ns - (seq_t)28 * ns + (seq_t)11) *
                          memo[i - 2];

            seq_t termC = (seq_t)2 *
                          ((seq_t)8 * ns * ns - (seq_t)17 * ns + (seq_t)7) *
                          memo[i - 3];

            seq_t termD = (ns - (seq_t)2) *
                          ((seq_t)2 * ns - (seq_t)1) *
                          memo[i - 4];

            seq_t rhs = termA + termC - termB - termD;

            memo[i] = rhs / lhs;
        }
#endif
        built = n;
    }
    return memo[n];
}
