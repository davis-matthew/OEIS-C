/*
 * A212885  Expansion of phi(q) * phi(-q)^2 in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A212885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 266, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A212885 (OEIS)
 *
 * Let c(d) = 2 if d is odd,
 *           5 if d ≡ 2 (mod 4),
 *           3 if d ≡ 0 (mod 4).
 * Let b(k) = Σ_{d|k} d * c(d).
 *
 * Then a(0) = 1 and for n ≥ 1
 *   a(n) = -(1/n) * Σ_{k=1}^{n} b(k) * a(n – k).
 *
 * The function `a` below computes the terms on demand and memoises
 * all previously computed values.  It is safe for both fixed‑size
 * signed integer types and arbitrary‑precision integer types.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *A = NULL;          /* a(n) values               */
static seq_t *B = NULL;          /* b(n) values               */
static seq_t *C = NULL;          /* c(n) values               */
static idx_t  max_n = 0;         /* largest index computed    */
static int    ready  = 0;        /* 0 = not initialised      */

/* Ensure that A[0..n], B[0..n] and C[0..n] are available. */
static void ensure_up_to(idx_t n)
{
    if (ready && n <= max_n)            /* already computed */
        return;

    /* allocate new larger arrays */
    size_t new_size = (size_t)n + 1;
    seq_t *newA = (seq_t*)calloc(new_size, sizeof(seq_t));
    seq_t *newB = (seq_t*)calloc(new_size, sizeof(seq_t));
    seq_t *newC = (seq_t*)calloc(new_size, sizeof(seq_t));

    if (ready) {
        /* copy old contents */
        size_t old_size = (size_t)max_n + 1;
        for (idx_t i = 0; i < old_size; ++i) {
            newA[i] = A[i];
            newB[i] = B[i];
            newC[i] = C[i];
        }
        free(A); free(B); free(C);
    }

    /* compute c(d) for new indices */
    for (idx_t d = max_n + 1; d <= n; ++d) {
        if (d & 1ULL)
            newC[d] = (seq_t)2;          /* odd */
        else if ((d & 3ULL) == 2)
            newC[d] = (seq_t)5;          /* d ≡ 2 (mod 4) */
        else
            newC[d] = (seq_t)3;          /* d ≡ 0 (mod 4) */
    }

    /* compute b(k) for new indices by a trivial divisor sum */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t d = 1; d <= k; ++d) {
            if (k % d == 0)
                sum += (seq_t)d * newC[d];
        }
        newB[k] = sum;
    }

    /* initialise a(0) if this is the first call */
    if (!ready)
        newA[0] = (seq_t)1;

    /* compute a(n) for all new indices */
    for (idx_t m = (ready ? max_n + 1 : 1); m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k)
            sum += newB[k] * newA[m - k];
        newA[m] = -(sum / (seq_t)m);
    }

    /* replace old arrays */
    A = newA;   B = newB;   C = newC;
    max_n = n;
    ready = 1;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return A[n];
}
