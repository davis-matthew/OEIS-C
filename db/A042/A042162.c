/*
 * A042162  Numerators of continued fraction convergents to sqrt(606).
 * https://oeis.org/A042162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(0)=24; for n>0: a(n)=c[n]*a(n-1)+a(n-2) where c[0]=24, and
 *            c[1…] are the continued‑fraction coefficients of sqrt(606)
 *            (period 20, last coefficient 48)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 22, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Continued‑fraction coefficients after the integer part 24.
   c[1] corresponds to a_1, c[2] to a_2, …  */
static const unsigned c_coeffs[] = {
    1, 1, 1, 1, 1, 1, 2, 1, 9, 8,
    9, 1, 2, 1, 1, 1, 1, 1, 1, 48
};
static const size_t C_LEN = sizeof(c_coeffs)/sizeof(c_coeffs[0]);

/* Cache of already computed numerators.  */
static seq_t *cache   = NULL;   /* array of p_k */
static size_t cap    = 0;       /* allocated capacity */
static size_t size_  = 0;       /* number of elements stored (size_ = last_index+1) */

/* Ensure that the cache can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cap)
        return;
    size_t new_cap = cap ? cap : 1;
    while (new_cap < need)
        new_cap <<= 1;               /* double until enough */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* fatal error on allocation failure */
    cache = tmp;
    cap   = new_cap;
}

/* Return the n‑th term of the sequence (0‑based). */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;           /* n is non‑negative and fits in size_t for our use */

    /* Initialise the cache with the first term if empty. */
    if (size_ == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)24;         /* a(0) = 24 */
        size_ = 1;
    }

    /* Return cached value if already computed. */
    if (idx < size_)
        return cache[idx];

    /* Compute successive terms up to idx. */
    for (size_t k = size_; k <= idx; ++k) {
        seq_t coeff;
        if (k == 0)
            coeff = (seq_t)24;                   /* a_0 = 24 */
        else
            coeff = (seq_t)c_coeffs[(k - 1) % C_LEN];/* a_k for k>=1 */

        seq_t prev  = cache[k - 1];
        seq_t prev2 = (k >= 2) ? cache[k - 2] : (seq_t)1; /* a(-1) = 1 */

        ensure_capacity(k + 1);
        cache[k] = coeff * prev + prev2;      /* recurrence: p_k = a_k*p_{k-1} + p_{k-2} */
    }

    size_ = idx + 1;     /* update cache length */
    return cache[idx];
}
