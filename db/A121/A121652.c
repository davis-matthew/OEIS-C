/*
 * A121652  a(n) is the cube of the coefficient of x^n in 1/(1 - x*A(x^3)),
 *          where g.f. A(x) = Sum_{n>=0} a(n)*x^n.
 * https://oeis.org/A121652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let b(n) denote the cube‐root of a(n) (so a(n) = b(n)^3). The
 *            sequence satisfies b(0)=1 and for n≥1, b(n) =
 *            Σ_{k=0}^{⌊(n-1)/3⌋} b(n‑1‑3k) · b(k)^3 . The function below
 *            computes a(n)=b(n)^3 using this recurrence
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *b = NULL;        /* b[n]  – the cube root of a(n)            */
static seq_t *cube = NULL;     /* cube[n] = b[n]^3                         */
static size_t cap = 0;         /* current capacity of the arrays           */
static idx_t computed = 0;     /* highest n that has been computed yet     */

seq_t a(idx_t n)
{
    /* Ensure the arrays are big enough */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *b_new = (seq_t *)malloc(new_cap * sizeof(seq_t));
        seq_t *cube_new = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (b && cap > 0) {
            memcpy(b_new, b, cap * sizeof(seq_t));
            memcpy(cube_new, cube, cap * sizeof(seq_t));
            free(b);
            free(cube);
        }
        b = b_new;
        cube = cube_new;
        cap = new_cap;
    }

    /* Base case initialization (only once) */
    if (computed == 0 && cap >= 1) {
        b[0] = (seq_t)1;
        cube[0] = (seq_t)1;   /* 1^3 = 1 */
    }

    /* Compute terms up to n if needed */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t maxk = (i - 1) / 3;       /* floor((i-1)/3) */
        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t idx = i - 1 - 3 * k;  /* i‑1‑3k >= 0 by definition of maxk */
            sum += b[idx] * cube[k];
        }
        b[i] = sum;
        cube[i] = sum * sum * sum;     /* compute b(i)^3 */
    }
    computed = n;

    return cube[n];
}
