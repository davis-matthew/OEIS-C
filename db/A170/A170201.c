/*
 * A170201  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A170201 – Number of reduced words of length n in a Coxeter group on
   48 generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
   Recurrence (n ≥ 39):
      a(n) = (46 * Σ_{k=1}^{38} a(n-k) + a(n-39)) / 1081
   Initial values:
      a(0) = 1,  a(1) = 48
      a(n) = 48 * 47^(n-1)   for 2 ≤ n ≤ 38   */

/* C/C++ compatible implementation */
#include <stdlib.h>   /* malloc, free */

static inline seq_t pow_nc(seq_t base, idx_t exp);
static inline seq_t sum_subseq(const seq_t *a, idx_t idx, idx_t len);

seq_t a(idx_t n)
{
    /* Handle trivial small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)48;

    /* Allocate array to hold values 0 … n */
    seq_t *data = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!data) return (seq_t)0;          /* allocation failure (unlikely) */

    data[0] = (seq_t)1;
    data[1] = (seq_t)48;

    /* Compute 2 … min(n,38) exactly by successive multiplication */
    if (n >= 2) {
        seq_t pow47 = (seq_t)47;           /* 47^(1) */
        for (idx_t i = 2; i <= (n < 38 ? n : 38); ++i) {
            data[i] = (seq_t)48 * pow47;  /* 48 * 47^(i-1) */
            pow47 *= (seq_t)47;           /* next power of 47 */
        }
    }

    /* Recurrence for n ≥ 39 */
    if (n >= 39) {
        for (idx_t i = 39; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 38; ++k) sum += data[i - k];
            data[i] = ((seq_t)46 * sum + data[i - 39]) / (seq_t)1081;
        }
    }

    seq_t result = data[n];
    free(data);
    return result;
}
