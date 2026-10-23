/*
 * A140046  G.f. satisfies: A(x) = x/(1 - A(x+x^2)).
 * https://oeis.org/A140046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1. * For n > 1, let * c(k) = Σ_{m=ceil(k/2)}^k a(m) *
 *            C(m, k-m) * (C(m,j) = binomial coefficient). * Then * a(n) =
 *            Σ_{i=1}^{n-1} a(i) * c(n-i). * The implementation maintains
 *            arrays a[] and c[] that are updated * incrementally as new terms
 *            are computed. The memory is doubled * when a large...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_a = NULL;          /* a[1 .. computed_N]            */
static seq_t *seq_c = NULL;          /* c[0 .. 2*computed_N] (0 unused) */
static idx_t computed_N = 0;         /* largest index for which a[] is known */
static size_t seq_c_size = 0;        /* current allocated size of seq_c */

/* Ensure that seq_a and seq_c are large enough for indices up to new_N. */
static void enlarge_arrays(idx_t new_N)
{
    if (new_N <= computed_N)
        return;

    /* Grow seq_a */
    size_t new_a_size = new_N + 1; /* indices 0 .. new_N */
    seq_a = (seq_t *)realloc(seq_a, new_a_size * sizeof(seq_t));
    for (idx_t i = computed_N + 1; i <= new_N; ++i)
        seq_a[i] = 0;

    /* Grow seq_c to hold indices up to 2*new_N */
    size_t required_c_size = 2 * new_N + 1;
    if (required_c_size > seq_c_size) {
        seq_c = (seq_t *)realloc(seq_c, required_c_size * sizeof(seq_t));
        for (size_t i = seq_c_size; i < required_c_size; ++i)
            seq_c[i] = 0;
        seq_c_size = required_c_size;
    }
}

/* Add the contributions of a_m into the c[] array.
 * After this call, for all k ∈ [m, 2m] we have
 *   c[k] += a_m * C(m, k-m). */
static void add_contributions(idx_t m)
{
    seq_t val = seq_a[m];
    seq_t binom = 1; /* C(m,0) */
    for (idx_t j = 0; j <= m; ++j) {
        idx_t k = m + j;          /* k runs from m to 2m */
        seq_c[k] += val * binom;
        if (j == m)
            break;
        seq_t num = binom * (seq_t)(m - j);
        binom = num / (seq_t)(j + 1);
    }
}

/* Public routine demanded by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* unused offset */

    /* Lazily initialise for first call. */
    if (computed_N == 0) {
        enlarge_arrays(1);
        seq_a[1] = 1;             /* a(1) = 1 */
        add_contributions(1);     /* set c[1] and c[2] */
        computed_N = 1;
    }

    /* Compute terms up to n if necessary. */
    if (n > computed_N) {
        enlarge_arrays(n);
        for (idx_t i = computed_N + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t j = 1; j < i; ++j) {
                sum += seq_a[j] * seq_c[i - j];
            }
            seq_a[i] = sum;
            add_contributions(i);
        }
        computed_N = n;
    }

    return seq_a[n];
}
