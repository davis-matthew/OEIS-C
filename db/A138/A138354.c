/*
 * A138354  Central moment sequence of tr(A^4) in USp(4).
 * https://oeis.org/A138354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0..n} (-1)^i * binomial(n,i) * binom(i,⌊i/2⌋)^2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the binomial transform of A018224, where A018224(i) =
   binom(i,⌊i/2⌋)^2. */

#include <stdlib.h>

static seq_t *central = NULL;   /* central[i] = binom(i, floor(i/2)) */
static size_t central_len = 0;

/* Ensure central[i] is available for 0..n (inclusive). */
static void ensure_central(idx_t n)
{
    if (central_len > n)
        return;

    /* Allocate or grow the array. */
    size_t new_len = (size_t)n + 1;
    central = (seq_t *)realloc(central, new_len * sizeof(seq_t));

    /* Compute central values that are missing. */
    idx_t start = (central_len == 0) ? 0 : (idx_t)central_len;
    if (start == 0) {
        central[0] = (seq_t)1;
        ++start;
    }
    for (idx_t i = start; i <= n; ++i) {
        seq_t prev = central[i - 1];
        if ((i & 1) == 0) {          /* i even */
            central[i] = (prev * (seq_t)i) / (seq_t)(i / 2);
        } else {                    /* i odd */
            central[i] = (prev * (seq_t)i) / (seq_t)(i / 2 + 1);
        }
    }
    central_len = new_len;
}

/* Central moment sequence of tr(A^4) in USp(4). */
seq_t a(idx_t n)
{
    /* Precompute central binomial coefficients up to n. */
    ensure_central(n);

    seq_t sum = (seq_t)0;
    seq_t bin = (seq_t)1;            /* binomial(n,0) */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t square = central[i] * central[i];  /* binom(i,⌊i/2⌋)^2 */
        seq_t term  = bin * square;

        if ((i & 1) == 0)          /* (+) sign */
            sum += term;
        else                       /* (-) sign */
            sum -= term;

        if (i < n) {
            /* Update binomial coefficient to binomial(n,i+1). */
            bin = bin * (seq_t)(n - i) / (seq_t)(i + 1);
        }
    }

    return sum;
}
