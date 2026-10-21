/*
 * A202826  E.g.f.: exp( 1/(1-x)^3 - 1 ).
 * https://oeis.org/A202826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from A'(x)=3/(1-x)^4*A(x):
   a(0) = 1;
   for n >= 0:
     a(n+1) = Σ_{j=1}^{n+1} j · C(j+2,2) · (n)_{j-1} · a(n+1-j)
   where (n)_k = n·(n-1)·…·(n-k+1) (falling factorial).
   All intermediate values stay integers. */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *A = NULL;
    static size_t cap = 0;
    static idx_t computed = 0;      /* highest index already computed */

    /* Allocate or grow cache */
    if (A == NULL) {
        cap = 1;
        A = (seq_t *)malloc(cap * sizeof(seq_t));
        A[0] = (seq_t)1;           /* a(0) = 1 */
    } else if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing entries up to n */
    for (idx_t next = computed + 1; next <= n; ++next) {
        idx_t n_prev = next - 1;     /* n in the recurrence */
        seq_t sum = (seq_t)0;
        seq_t fact = (seq_t)1;       /* (n)_0 = 1 */

        for (idx_t j = 1; j <= next; ++j) {
            /* compute j * C(j+2,2) as a 64‑bit integer */
            uint64_t mult = (uint64_t)j * ((uint64_t)(j + 2) * (j + 1) / 2ULL);
            /* term = a[next-j] * mult * fact */
            seq_t term = A[next - j];
            term *= (seq_t)mult;
            term *= fact;
            sum += term;
            /* update fact for next j:
               fact := fact * (n - j + 1)  for j < next   (i.e. t < n) */
            if (j < next)
                fact *= (n_prev - j + 1);
        }
        A[next] = sum;
    }
    computed = n >= computed ? n : computed;

    return A[n];
}
