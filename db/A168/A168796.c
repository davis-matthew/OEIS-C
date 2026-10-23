/*
 * A168796  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq: number of reduced words of length n in the Coxeter group
 * on 23 generators with relations S_i^2 = (S_i S_j)^19 = I.
 *
 * The sequence a(n) satisfies the linear recurrence
 *
 *     a(n) = 21 a(n-1) + 21 a(n-2) + … + 21 a(n-18) - 231 a(n-19)   (n ≥ 19)
 *
 * The first 20 terms (a(0) … a(19)) are obtained by expanding the generating
 * function
 *
 *     G(t) = (t^19 + 2 t^18 + … + 2 t + 1) /
 *            (1 - 21 t - 21 t^2 - … - 21 t^18 + 231 t^19).
 *
 * The implementation builds the sequence on demand and caches all computed
 * terms so that each coefficient is evaluated at most once.
 */

#include <stdlib.h>
#include <stdbool.h>

/* The types seq_t and idx_t are supplied by the build system. */
static seq_t *sequence = NULL;      /* cached values a(0), a(1), … */
static size_t seq_len = 0;          /* number of cached terms */
static size_t seq_cap = 0;          /* capacity of the cache */

/* Ensure the cache has at least 'new_cap' elements. */
static void
ensure_capacity(size_t new_cap)
{
    if (new_cap <= seq_cap)
        return;

    size_t cap = seq_cap ? seq_cap : 64;
    while (cap < new_cap)
        cap <<= 1;                 /* grow exponentially */

    seq_t *new_mem = (seq_t *)realloc(sequence, cap * sizeof(seq_t));
    if (!new_mem)
        abort();                   /* allocation failed */

    sequence = new_mem;
    seq_cap = cap;
}

/* Compute the first 20 terms (a(0) … a(19)) from the generating function
 * and store them in the cache.  This function is idempotent. */
static void
init_sequence(void)
{
    if (seq_len > 0)
        return;                    /* already initialized */

    /* Coefficients of the denominator: 1 - 21 t - … - 21 t^18 + 231 t^19 */
    const int d_coeff[20] = {
        1,  /* t^0 */
        -21, -21, -21, -21, -21, -21, -21, -21, -21,
        -21, -21, -21, -21, -21, -21, -21, -21, -21, 231
    };

    /* Coefficients of the numerator: 1 + 2 t + … + 2 t^18 + t^19 */
    const seq_t n_coeff[20] = {
        (seq_t)1,
        (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2,
        (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2,
        (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2,
        (seq_t)1
    };

    ensure_capacity(20);           /* we need room for a(0)…a(19) */

    for (idx_t n = 0; n <= 19; ++n) {
        if (n == 0) {
            sequence[0] = n_coeff[0];   /* a(0) = 1 */
        } else {
            seq_t val = n_coeff[n];
            for (idx_t k = 1; k <= n; ++k) {
                int dk = d_coeff[k];
                if (dk < 0)
                    val += (seq_t)(-dk) * sequence[n - k];
                else
                    val -= (seq_t)dk * sequence[n - k];
            }
            sequence[n] = val;
        }
    }

    seq_len = 20;                  /* indices 0…19 are now available */
}

/* Public function: returns a(n).  Values are cached after first computation. */
seq_t
a(idx_t n)
{
    init_sequence();

    if (n < seq_len)
        return sequence[n];

    /* Extend the cache up to index n using the recurrence. */
    ensure_capacity(n + 1);

    for (idx_t idx = seq_len; idx <= n; ++idx) {
        seq_t val = 0;
        /* a(idx) = 21*a(idx-1) + ... + 21*a(idx-18) - 231*a(idx-19) */
        for (idx_t k = 1; k <= 18; ++k)
            val += (seq_t)21 * sequence[idx - k];
        val -= (seq_t)231 * sequence[idx - 19];
        sequence[idx] = val;
    }

    seq_len = n + 1;
    return sequence[n];
}
