/*
 * A149589  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 1), (-1, 0,
 *          1), (-1, 1, 1), (0, 0, -1), (1, 1, 1)}.
 * https://oeis.org/A149589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = sum_{k=0..⌊n/2⌋} binom(n,k) * (n-2k+1)/(n-k+1) * U(n-k)
 *            where U(m) = sum_{j=0..⌊m/2⌋} binom(m,j) * (m-2j+1)/(m-j+1) *
 *            3^j. The function implements this double‑sum using incremental
 *            binomial and pre‑computed powers of 3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static storage for intermediate values */
static seq_t *U_arr = NULL;      /* U(m) values */
static size_t U_cap = 0;

static seq_t *pow3_arr = NULL;   /* powers of 3 */
static size_t pow3_cap = 0;

static seq_t *a_arr = NULL;      /* a(n) sequence */
static size_t a_cap = 0;
static size_t a_count = 0;       /* number of a values stored */

/* Ensure capacity for an array of seq_t elements, resizing with
   doubling strategy.  The array is reallocated with malloc/realloc. */
static void ensure_cap(seq_t **arr, size_t *cap, size_t needed, size_t elem_size)
{
    if (*cap <= needed) {
        size_t new_cap = *cap ? *cap << 1 : 4;
        while (new_cap <= needed) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * elem_size);
        *cap = new_cap;
    }
}

/* Ensure powers of 3 up to index n are available and the array has
   sufficient capacity.  All entries are filled. */
static void ensure_pow3_cap(size_t n)
{
    if (pow3_cap <= n) {
        size_t new_cap = pow3_cap ? pow3_cap << 1 : 4;
        while (new_cap <= n) new_cap <<= 1;
        pow3_arr = (seq_t *)realloc(pow3_arr, new_cap * sizeof(seq_t));
        for (size_t j = 0; j < new_cap; ++j) {
            if (j == 0)
                pow3_arr[0] = (seq_t)1;
            else
                pow3_arr[j] = pow3_arr[j-1] * (seq_t)3;
        }
        pow3_cap = new_cap;
    }
}

/* Compute U(m) for all m in [U_count .. n].  U_count is the next
   index that has not yet been computed. */
static size_t U_count = 0;
static void compute_U_up_to(size_t n)
{
    if (n < U_count) return;
    ensure_cap(&U_arr, &U_cap, n, sizeof(seq_t));

    for (size_t m = U_count; m <= n; ++m) {
        size_t max_k = m >> 1;
        ensure_pow3_cap(max_k);

        seq_t sum = 0;
        seq_t binom = 1;  /* binom(m,0) */
        for (size_t k = 0; k <= max_k; ++k) {
            seq_t term = binom;
            term = term * (seq_t)(m - 2*k + 1);
            term = term / (seq_t)(m - k + 1);
            term = term * pow3_arr[k];
            sum += term;

            if (k < max_k) {
                binom = binom * (seq_t)(m - k);
                binom = binom / (seq_t)(k + 1);
            }
        }
        U_arr[m] = sum;
    }
    U_count = n + 1;
}

/* Compute a(n) for all n in [a_count .. N].  a_count is the next
   index that has not yet been computed. */
static void compute_a_up_to(idx_t n)
{
    if (n < 0) return;
    size_t N = (size_t)n;
    ensure_cap(&a_arr, &a_cap, N, sizeof(seq_t));

    /* U values needed up to N */
    compute_U_up_to(N);

    for (size_t curr = a_count; curr <= N; ++curr) {
        size_t max_d = curr >> 1;
        seq_t sum = 0;
        seq_t binom = 1;  /* binom(curr,0) */
        for (size_t d = 0; d <= max_d; ++d) {
            seq_t term = binom;
            term = term * (seq_t)(curr - 2*d + 1);
            term = term / (seq_t)(curr - d + 1);
            term = term * U_arr[curr - d];
            sum += term;

            if (d < max_d) {
                binom = binom * (seq_t)(curr - d);
                binom = binom / (seq_t)(d + 1);
            }
        }
        a_arr[curr] = sum;
    }
    a_count = N + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    compute_a_up_to(n);
    return a_arr[(size_t)n];
}
