/*
 * A224271  Number of set partitions of {1,2,...,n} such that the element 1 is
 *          in an odd-sized block.
 * https://oeis.org/A224271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} binom(n-1, 2k) * Bell(n-2k-1).
 *            Implementation: compute Bell numbers incrementally
 *            (B_{m+1}=∑_{k=0}^{m} binom(m,k)*B_k) and use them to evaluate
 *            the sum for each requested n
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 25, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *Bell = NULL;      /* Bell numbers B[0] .. B[max]   */
static idx_t  Bell_max = 0;     /* largest index with known B[i] */
static size_t Bell_cap = 0;     /* allocated capacity */

static seq_t *A   = NULL;      /* a[1] .. a[max]  */
static idx_t  A_max = 0;       /* largest index with known a[i] */
static size_t A_cap = 0;       /* allocated capacity */

/* ensure Bell[0] is set to 1 before any use */
static void init_bell(void)
{
    if (Bell == NULL) {
        Bell_cap = 1;
        Bell = (seq_t *)malloc(sizeof(seq_t));
        if (!Bell) exit(1);
        Bell[0] = 1;
        Bell_max = 0;
    }
}

static void ensure_bell(idx_t upto)
{
    /* we need Bell[0..upto]  (upto = n-1 for a(n)) */
    if (Bell_max >= upto) return;
    if (Bell_cap < upto + 1) {
        Bell_cap = upto + 1;
        Bell = (seq_t *)realloc(Bell, Bell_cap * sizeof(seq_t));
        if (!Bell) exit(1);
    }
    for (idx_t m = Bell_max; m < upto; ++m) {
        seq_t sum   = 0;
        seq_t binom = 1;
        for (idx_t k = 0; k <= m; ++k) {
            sum += binom * Bell[k];
            if (k < m) {
                binom = binom * ((seq_t)(m - k));
                binom = binom / ((seq_t)(k + 1));
            }
        }
        Bell[m + 1] = sum;
    }
    Bell_max = upto;
}

static void ensure_a(idx_t upto)
{
    if (A_max >= upto) return;
    if (A_cap < upto) {
        A_cap = upto;
        A = (seq_t *)realloc(A, A_cap * sizeof(seq_t));
        if (!A) exit(1);
    }
    for (idx_t i = A_max + 1; i <= upto; ++i) {
        idx_t m = i - 1;
        seq_t res   = 0;
        seq_t binom = 1;
        int parity  = m & 1;
        for (idx_t j = 0; j <= m; ++j) {
            if (((int)j & 1) == parity)
                res += binom * Bell[j];
            if (j < m) {
                binom = binom * ((seq_t)(m - j));
                binom = binom / ((seq_t)(j + 1));
            }
        }
        A[i - 1] = res;
    }
    A_max = upto;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1, undefined for n=0 */
    init_bell();
    ensure_bell(n - 1);                  /* need Bell[0..n-1]   */
    ensure_a(n);                         /* compute a up to n    */
    return A[n - 1];
}
