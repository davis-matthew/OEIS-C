/*
 * A204649  Number of (n+1) X 7 0..1 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) nondecreasing in
 *          column and row directions, respectively.
 * https://oeis.org/A204649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 20*a(n-2) + 28*a(n-3) - 14*a(n-4) - 14*a(n-5)
 *            + 28*a(n-6) - 20*a(n-7) + 7*a(n-8) - a(n-9), n > 13 a(1) … a(13)
 *            are given below
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 75, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial known terms (n = 1 … 13) */
    static const seq_t init[14] = {
        0ULL,          /* unused: 0 */
        132ULL, 200ULL, 434ULL, 938ULL, 1918ULL,
        3702ULL, 6780ULL, 11868ULL, 19969ULL,
        32450ULL, 51134ULL, 78404ULL, 117324ULL
    };

    if (n <= 13) {
        /* cast the index to the array index type */
        return init[(size_t)n];
    }

    /* allocate temporary array to hold values up to n */
    size_t N = (size_t)n;
    seq_t *f = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!f) { /* allocation failed */
        /* terminate program, as required by the build environment */
        exit(1);
    }

    /* fill the known initial values */
    for (size_t i = 1; i <= 13; ++i) {
        f[i] = init[i];
    }

    /* compute terms n = 14 … N using the recurrence */
    for (size_t i = 14; i <= N; ++i) {
        f[i] = (seq_t)7 * f[i-1]
             - (seq_t)20 * f[i-2]
             + (seq_t)28 * f[i-3]
             - (seq_t)14 * f[i-4]
             - (seq_t)14 * f[i-5]
             + (seq_t)28 * f[i-6]
             - (seq_t)20 * f[i-7]
             + (seq_t)7  * f[i-8]
             - f[i-9];
    }

    seq_t result = f[N];
    free(f);
    return result;
}
