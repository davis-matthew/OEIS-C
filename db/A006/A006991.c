/*
 * A006991  Primitive congruent numbers.
 * https://oeis.org/A006991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th squarefree k satisfying Tunnell's criterion
 *            (equality conditional on BSD): k odd: #{2x^2+y^2+8z^2=k} =
 *            2*#{2x^2+y^2+32z^2=k}; k even: #{4x^2+y^2+8z^2=k/2} =
 *            2*#{4x^2+y^2+32z^2=k/2}; counts obtained by a sieve over
 *            representations
 * Range:     u8 n <= 95, u16 n <= 3503, u32 n <= 3503, u64 n <= 3503,
 *            u128 n <= 3503, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static long long *terms = 0;
static long long nterms = 0;

/* add representation counts of c1*x^2 + y^2 + c3*z^2 for all values <= L */
static void count_form(int *cnt, long long L, long long c1, long long c3)
{
    long long x, y, z;
    for (x = 0; c1 * x * x <= L; x++) {
        long long vx = c1 * x * x;
        int mx = x ? 2 : 1;
        for (z = 0; vx + c3 * z * z <= L; z++) {
            long long vxz = vx + c3 * z * z;
            int mxz = mx * (z ? 2 : 1);
            for (y = 0; vxz + y * y <= L; y++)
                cnt[vxz + y * y] += mxz * (y ? 2 : 1);
        }
    }
}

static void build(long long L)
{
    long long half = L / 2, k, m;
    int *a1 = (int *)calloc((size_t)(L + 1), sizeof(int));
    int *b1 = (int *)calloc((size_t)(L + 1), sizeof(int));
    int *a2 = (int *)calloc((size_t)(half + 1), sizeof(int));
    int *b2 = (int *)calloc((size_t)(half + 1), sizeof(int));
    bool *nsf = (bool *)calloc((size_t)(L + 1), sizeof(bool));
    count_form(a1, L, 2, 8);
    count_form(b1, L, 2, 32);
    count_form(a2, half, 4, 8);
    count_form(b2, half, 4, 32);
    for (k = 2; k * k <= L; k++)
        for (m = k * k; m <= L; m += k * k)
            nsf[m] = true;
    free(terms);
    terms = (long long *)malloc((size_t)(L + 1) * sizeof(long long));
    nterms = 0;
    for (k = 1; k <= L; k++) {
        bool ok;
        if (nsf[k]) continue;
        if (k & 1) ok = (a1[k] == 2 * b1[k]);
        else ok = (a2[k / 2] == 2 * b2[k / 2]);
        if (ok) terms[nterms++] = k;
    }
    free(a1); free(b1); free(a2); free(b2); free(nsf);
}

seq_t a(idx_t n)
{
    long long L = 1024;
    if ((long long)n <= nterms) return (seq_t)terms[n - 1];
    while (1) {
        build(L);
        if ((long long)n <= nterms) break;
        L *= 2;
    }
    return (seq_t)terms[n - 1];
}
