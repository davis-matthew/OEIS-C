/*
 * A046748  Row sums of triangle A046521.
 * https://oeis.org/A046748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 2*C_{n-1} where C_k is the k‑th Catalan number
 *            defined by C_0 = 1, C_{k+1} = C_k * 2*(2*k+1)/(k+2) (k >= 0)
 *            Initial value: a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

/* maximum n that will be queried (based on tests) */
#define MAX_CACHE 10000

/* static tables to memoise both a(n) and Catalan numbers C_n   */
static seq_t a_cache[MAX_CACHE + 1];
static seq_t cat_cache[MAX_CACHE + 1];
/* indices of the highest element that has been computed (0‑based) */
static idx_t last_a   = 0;   /* a_cache[0] is known (a(0)=1)   */
static idx_t last_cat = 0;   /* cat_cache[0] is known (C_0=1) */

/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Directly return the base case without any work */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Ensure base entries are in place (they start as zero) */
    if (a_cache[0] == 0) {
        a_cache[0] = (seq_t)1;
    }
    if (cat_cache[0] == 0) {
        cat_cache[0] = (seq_t)1;
    }

    /* Grow the Catalan table up to index n-1 if needed */
    while (last_cat < n - 1) {
        idx_t k = last_cat;                /* current maximum known Catalan index */
        seq_t prev = cat_cache[k];         /* C_k                 */
        /* C_{k+1} = C_k * 2*(2*k+1) / (k+2) */
        seq_t mult   = (seq_t)2 * (seq_t)(2 * k + 1);
        seq_t numer  = prev * mult;
        seq_t denom  = (seq_t)(k + 2);
        seq_t next   = numer / denom;
        cat_cache[k + 1] = next;
        last_cat = k + 1;
    }

    /* Compute a(n) values sequentially up to n */
    while (last_a < n) {
        idx_t i = last_a;                 /* last computed a index */
        /* a_{i+1} = 5*a_i - 2*C_i */
        seq_t val = a_cache[i] * (seq_t)5 - cat_cache[i] * (seq_t)2;
        a_cache[i + 1] = val;
        last_a = i + 1;
    }

    return a_cache[n];
}
