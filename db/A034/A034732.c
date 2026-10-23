/*
 * A034732  Dirichlet convolution of b_n=1 with Bell numbers.
 * https://oeis.org/A034732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n} B_{d-1} , where B_k are the Bell numbers (B_0 =
 *            1)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------  includes  ------------------------------------- */
#include <stddef.h>        /* size_t, NULL */
#include <stdlib.h>        /* malloc, realloc */
#include "../utils/mul_small.h"

/* --------------------  static data  ----------------------------------- */
static seq_t *Bell = NULL;          /* Bell[0..] */
static size_t Bell_cap = 0;
static idx_t  Bell_len = 0;         /* number of Bell values stored */

static seq_t *A = NULL;             /* a[0..]  (a[0] unused) */
static size_t A_cap = 0;
static idx_t  max_a = 0;            /* largest n for which a[n] is computed */

/* --------------------  helpers  ------------------------------------- */
static void compute_bell(idx_t upto)
{
    if (Bell_len > upto) return;    /* already computed */

    /* ensure capacity */
    if (Bell_cap < (size_t)upto + 1) {
        size_t new_cap = (size_t)upto + 1;
        seq_t *tmp = (seq_t*)realloc(Bell, new_cap * sizeof(seq_t));
        Bell = tmp;
        Bell_cap = new_cap;
    }

    if (Bell_len == 0) {
        Bell[0] = 1;                /* B_0 */
        Bell_len = 1;
    }

    for (idx_t n = Bell_len - 1; n < upto; ++n) {
        seq_t sum   = 0;
        seq_t binom = 1;            /* C(n,0) */
        for (idx_t k = 0; k <= n; ++k) {
            sum += binom * Bell[k]; /* binom * B_k */

            if (k < n) {
                /* binom = C(n,k) * (n-k) / (k+1)  */
                seq_t tmp = mul_small(binom, (unsigned)(n - k));
                binom = tmp / (unsigned)(k + 1);
            }
        }
        Bell[n + 1] = sum;          /* B_{n+1} */
    }
    Bell_len = upto + 1;
}

static void compute_A_up_to(idx_t n)
{
    if (n <= max_a) return;         /* already computed */

    /* ensure capacity */
    size_t new_cap = (size_t)n + 1;
    if (A_cap < new_cap) {
        seq_t *tmp = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
        A = tmp;
        A_cap = new_cap;
    }

    /* initialize and compute a[1..n] from scratch */
    for (idx_t i = 0; i <= n; ++i) A[i] = 0;
    for (idx_t d = 1; d <= n; ++d) {
        seq_t val = Bell[d - 1];    /* B_{d-1} */
        for (idx_t m = d; m <= n; m += d) {
            A[m] += val;
        }
    }
    max_a = n;
}

/* --------------------  public function  ------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* sequence defined for n >= 1 */

    compute_bell(n - 1);
    compute_A_up_to(n);

    return A[n];
}
