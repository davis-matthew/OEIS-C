/*
 * A087648  a(n) = (1/2)*(Bell(n+2)+Bell(n+1)-Bell(n)).
 * https://oeis.org/A087648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Bell(n+2)+Bell(n+1)-Bell(n))/2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Bell numbers on‑the‑fly with dynamic caching.
   Uses the recurrence B(n+1) = Σ_{k=0}^{n} C(n,k)*B(k). */


/* Include standard library for memory management */
#include <stdlib.h>

/* External integer types (provided by the build) */
extern seq_t a(idx_t n);   /* declaration required for the compiler */

/* Internal state for Bell numbers */
static seq_t *bell = NULL;      /* dynamic array of Bell numbers */
static size_t bell_len = 0;     /* number of computed Bell entries */
static size_t bell_cap = 0;     /* current capacity of the array */

/* Ensure the bell array has capacity for indices up to 'needed'. */
static void ensure_capacity(size_t needed)
{
    if (bell_cap > needed)
        return;

    size_t new_cap = bell_cap ? bell_cap * 2 : 1;
    while (new_cap <= needed)
        new_cap *= 2;

    seq_t *new_arr = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
    /* zero‑initialize new elements */
    for (size_t i = bell_cap; i < new_cap; ++i)
        new_arr[i] = 0;

    bell = new_arr;
    bell_cap = new_cap;
}

/* Compute Bell numbers up to 'needed' (inclusive). */
static void compute_bell_up_to(size_t needed)
{
    if (bell_len == 0) {
        /* Base case: B0 = 1 */
        ensure_capacity(1);
        bell[0] = (seq_t)1;
        bell_len = 1;
    }

    if (bell_len > needed)
        return;

    for (size_t i = bell_len; i <= needed; ++i) {
        if (i == 0) {
            bell[0] = (seq_t)1;
            continue;
        }

        size_t N = i - 1;             /* we compute B_{i} = B_{N+1} */
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;       /* C(N,0) */

        for (size_t k = 0; k <= N; ++k) {
            sum += binom * bell[k];
            if (k == N) break;
            /* Update binom: C(N, k+1) = C(N, k) * (N-k) / (k+1) */
            binom = binom * ((seq_t)(N - k)) / ((seq_t)(k + 1));
        }
        bell[i] = sum;
    }
    bell_len = needed + 1;
}

seq_t a(idx_t n)
{
    /* We need Bell numbers up to index n+2 */
    size_t needed = (size_t)n + 2;
    ensure_capacity(needed);
    compute_bell_up_to(needed);

    seq_t val = bell[needed] + bell[needed - 1] - bell[needed - 2];
    val = val / ((seq_t)2);
    return val;
}
