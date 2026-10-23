/*
 * A144155  Bell convolution triangle, T(n,k) = A000110(n-k) * A000110(k).
 * https://oeis.org/A144155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) = Bell(r - c) * Bell(c) where r = floor((sqrt(8*n+1)-1)/2)
 *            and c = n - r*(r+1)/2
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Bell numbers are computed up to the largest required index on demand.
   The maximum r for n ≈ 10 000 is ≈ 140, so only few hundred values are
   needed.  The routine uses only integer arithmetic. */

#include <stdlib.h>
#include <stddef.h>

/* static storage for Bell numbers */
static seq_t *bell = NULL;
static size_t bell_capacity = 0;  /* allocated size of the array          */
static idx_t bell_size = 0;       /* number of Bell values already computed */

/* Ensure that Bell numbers 0 … n are computed. */
static void ensure_bell(idx_t n)
{
    if (n < bell_size)
        return;                     /* already available */

    /* grow the array */
    size_t new_cap = (bell_capacity == 0) ? 1 : bell_capacity;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *new_arr = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
    if (!new_arr)                    /* memory allocation failure */
        exit(EXIT_FAILURE);
    bell        = new_arr;
    bell_capacity = new_cap;

    /* initialise the array if it was empty */
    if (bell_size == 0) {
        bell[0] = (seq_t)1;          /* B0 = 1 */
        bell_size = 1;
    }

    /* compute Bell numbers up to n */
    for (idx_t i = bell_size; i <= n; ++i) {
        idx_t m = i - 1;             /* we compute B_i from B_0 … B_{i-1} */
        seq_t sum   = 0;
        seq_t binom = (seq_t)1;      /* binomial(n, 0) = 1 */
        sum += binom * bell[0];      /* k = 0 */

        for (idx_t k = 1; k <= m; ++k) {
            binom = binom * (seq_t)(m - k + 1);
            binom = binom / (seq_t)k;          /* integer division   */
            sum += binom * bell[k];
        }
        bell[i] = sum;
    }
    bell_size = n + 1;
}

/* The sequence function */
seq_t a(idx_t n)
{
    /* Find row r and column c for the row‑major enumeration. */
    idx_t r = 0;
    /* r is the largest integer such that r*(r+1)/2 <= n.
       Increment r until the next start index would be larger than n. */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;          /* 0 <= c <= r */

    /* Ensure required Bell numbers are available. */
    ensure_bell(r);

    /* T(r,c) = Bell(r-c) * Bell(c) */
    seq_t result = bell[r - c] * bell[c];
    return result;
}
