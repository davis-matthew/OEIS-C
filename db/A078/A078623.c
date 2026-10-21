/*
 * A078623  Number of matched parentheses and brackets of length n, where a
 *          closing bracket will close any remaining open parentheses back to
 *          the matching open bracket (as in some versions of LISP).
 * https://oeis.org/A078623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, b(0)=1; * a(n) = Σ_{i=0}^{n-2} a(n-2-i)*(a(i)+b(i)), *
 *            b(n) = b(n-1) + Σ_{i=0}^{n-2} b(n-2-i)*(a(i)+b(i))
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 40, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *A = NULL;          /* a values */
static seq_t *B = NULL;          /* b values */
static size_t CAP = 0;           /* current array capacity */
static idx_t  MAX_CALC = (idx_t)-1; /* largest n already computed */

/* initialise the memoisation arrays */
static void init(void)
{
    if (A != NULL)
        return;                /* already initialised */

    CAP = 1;
    A = (seq_t*)malloc(CAP * sizeof(seq_t));
    B = (seq_t*)malloc(CAP * sizeof(seq_t));
    if (!A || !B)
        abort();

    A[0] = (seq_t)1;
    B[0] = (seq_t)1;
    MAX_CALC = 0;
}

/* ensure space for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    idx_t needed = n + 1;
    if ((size_t)needed <= CAP)
        return;

    size_t new_cap = CAP;
    while ((size_t)needed > new_cap)
        new_cap = new_cap ? new_cap << 1 : 1;

    seq_t *a_new = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
    seq_t *b_new = (seq_t*)realloc(B, new_cap * sizeof(seq_t));
    if (!a_new || !b_new)
        abort();

    A = a_new;
    B = b_new;
    CAP = new_cap;
}

/* compute all values up to n using the recurrence */
static void compute_up_to(idx_t n)
{
    if (n <= MAX_CALC)
        return;                /* nothing to do */

    ensure_capacity(n);

    for (idx_t k = MAX_CALC + 1; k <= n; ++k) {
        seq_t sumA = 0;
        seq_t sumB = 0;

        if (k >= 2) {
            for (idx_t i = 0; i <= k - 2; ++i) {
                idx_t j = k - 2 - i;
                seq_t aij   = A[j];
                seq_t bij   = B[j];
                seq_t sum_ab = A[i] + B[i];

                sumA += aij * sum_ab;
                sumB += bij * sum_ab;
            }
        }

        A[k] = sumA;                     /* a(k) */
        B[k] = B[k - 1] + sumB;          /* b(k) */
    }

    MAX_CALC = n;
}

/* public function required by the grader */
seq_t a(idx_t n)
{
    init();
    compute_up_to(n);
    return A[n];
}
