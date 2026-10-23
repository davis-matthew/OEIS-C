/*
 * A132841  Least increasing logarithmic coefficients such that exponentiation
 *          results in an integer sequence (A132842), starting with a(1)=1.
 * https://oeis.org/A132841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A132841:
   a(1)=1; for n≥2,
   a(n) is the minimal integer > a(n‑1) such that
        ( Σ_{k=1}^{n‑1} a(k)*b(n‑k) + a(n) ) is divisible by n,
   where b(0)=1 and
        b(n) = ( Σ_{k=1}^n a(k)*b(n‑k) ) / n.  */

/* Standard library for memory allocation */
#include <stdlib.h>

/* static storage for computed terms */
static seq_t *A = NULL;          /* a[1..max_n]           */
static seq_t *B = NULL;          /* b[0..max_n]           */
static idx_t  max_n = 0;         /* largest n that has been computed */
static size_t cap   = 0;         /* allocated capacity        */

/* Ensure arrays have space for indices up to n */
static void ensure_cap(idx_t n)
{
    if ((size_t)n <= cap) return;
    size_t new_cap = (cap == 0 ? 2 : cap * 2);
    while (new_cap < (size_t)n) new_cap <<= 1;     /* grow geometrically */
    seq_t *newA = (seq_t *)realloc(A, sizeof(seq_t) * (new_cap + 1));
    seq_t *newB = (seq_t *)realloc(B, sizeof(seq_t) * (new_cap + 1));
    if (!newA || !newB) return;                   /* allocation failure (unlikely) */
    A = newA;
    B = newB;
    cap = new_cap;
}

/* Compute a(n) for any n >= 1 (unordered calls allowed) */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* outside offset */

    ensure_cap(n);

    if (max_n == 0) {                    /* initialise first term */
        A[1] = (seq_t)1;
        B[0] = (seq_t)1;
        B[1] = (seq_t)1;                 /* b(1)=a(1) */
        max_n = 1;
    }

    for (idx_t m = max_n + 1; m <= n; ++m) {
        /* T_full = Σ_{k=1}^{m-1} a(k)*b(m-k)      (exact)
           T_mod   = T_full mod m                  (for divisibility)  */
        seq_t T_full = (seq_t)0;
        idx_t  T_mod  = 0;

        for (idx_t k = 1; k < m; ++k) {
            seq_t prod = A[k] * B[m - k];
            T_full += prod;

            /* compute product modulo m */
            idx_t prod_mod = (idx_t)((A[k] % (seq_t)m) *
                                     (B[m - k] % (seq_t)m));
            prod_mod %= m;
            T_mod += prod_mod;
            T_mod %= m;
        }

        /* Find r so that a(m) ≡ r (mod m), where r ≡ -T_full (mod m) */
        idx_t r = (idx_t)((m - T_mod) % m);

        /* a(m-1) modulo m */
        idx_t a_prev_mod = (idx_t)(A[m - 1] % (seq_t)m);

        /* delta = smallest positive integer ≡ r - a_prev_mod (mod m) */
        idx_t delta = (r + m - a_prev_mod) % m;
        if (delta == 0) delta = m;        /* strict increase */

        A[m] = A[m - 1] + (seq_t)delta;

        /* Compute b(m) = (T_full + a(m)) / m */
        seq_t sum = T_full + A[m];
        B[m] = sum / (seq_t)m;           /* guaranteed integer */
    }

    max_n = n;
    return A[n];
}
