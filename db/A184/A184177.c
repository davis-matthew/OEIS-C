/*
 * A184177  Number of set partitions of {1,2,...,n} having no blocks of the
 *          form {i,i+1,i+2}.
 * https://oeis.org/A184177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..floor(n/3)} (-1)^j * binomial(n-2j,j) *
 *            Bell(n-3j)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


/* e.g. a(3) = 4.  a(4) = 13, etc. */

/* Helper: compute binomial coefficient C(n,k) for idx_t arguments,
   using a multiplicative formula to avoid large intermediate factorials. */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (seq_t)(n - k + i) / (idx_t)i;
    return res;
}

/* Cache for Bell numbers.  bell[m] = B(m). */
static seq_t *bell   = NULL;
static size_t  bell_cap = 0;          /* allocated capacity in elements */
static idx_t   bell_next = 0;         /* first index not yet computed */

/* Ensure Bell numbers up to n are available in the cache. */
static void ensure_bell(idx_t n)
{
    if (n < bell_next) return;                 /* already computed */

    /* Grow the array if necessary. */
    if (bell_cap <= (size_t)n) {
        size_t new_cap = bell_cap ? bell_cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(bell, new_cap * sizeof(seq_t));
        bell = tmp;
        bell_cap = new_cap;
    }

    /* Initialise B(0)=1 on the very first call. */
    if (bell_next == 0) {
        bell[0] = 1;
        bell_next = 1;
    }

    /* Compute Bell numbers from bell_next up to n inclusive. */
    for (idx_t m = bell_next; m <= n; ++m) {
        seq_t sum = 0;
        seq_t bin = 1;                       /* C(m-1,0) */
        for (idx_t k = 0; k < m; ++k) {
            sum += bin * bell[k];
            if (k + 1 < m)
                bin = bin * (seq_t)(m - 1 - k) / (idx_t)(k + 1);
        }
        bell[m] = sum;
    }
    bell_next = n + 1;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    ensure_bell(n);                    /* Bell numbers up to n ready */

    seq_t even_sum = 0;
    seq_t odd_sum  = 0;
    idx_t max_j = n / 3;

    for (idx_t j = 0; j <= max_j; ++j) {
        seq_t term = binom_idx(n - 2 * j, j) * bell[n - 3 * j];
        if ((j & 1) == 0)
            even_sum += term;
        else
            odd_sum += term;
    }
    return even_sum - odd_sum;          /* even_sum ≥ odd_sum for all n */
}
