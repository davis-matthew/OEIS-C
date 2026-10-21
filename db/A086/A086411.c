/*
 * A086411  Greatest prime factor of 3-smooth numbers.
 * https://oeis.org/A086411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n=1, else 3 if the n‑th 3‑smooth number is divisible
 *            by 3, otherwise 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const double LOG2_3 = 1.5849625007211562;   /* log2(3) */

static idx_t        max_n   = 0;    /* highest n generated          */
static idx_t        i_ptr   = 1;    /* next index for multiplying by 2   */
static idx_t        j_ptr   = 1;    /* next index for multiplying by 3   */
static idx_t       *i_exp   = NULL;/* exponent of 2 for each number    */
static idx_t       *j_exp   = NULL;/* exponent of 3 for each number    */
static size_t        capacity = 0;/* allocated array size              */

/* Ensure that storage can hold at least indices up to n */
static void ensure_capacity(idx_t n)
{
    if (n <= capacity) return;
    size_t new_cap = n + 1;
    idx_t *new_i = (idx_t*)realloc(i_exp, new_cap * sizeof(idx_t));
    idx_t *new_j = (idx_t*)realloc(j_exp, new_cap * sizeof(idx_t));
    if (!new_i || !new_j) { exit(EXIT_FAILURE); }
    i_exp = new_i; j_exp = new_j;
    capacity = new_cap;
}

/* Compute the nth term of the sequence */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n<1 */

    ensure_capacity(n);

    /* Generate sequence up to n if needed */
    if (n > max_n) {
        if (max_n == 0) {           /* initialise first term  */
            i_exp[1] = 0;
            j_exp[1] = 0;
            max_n   = 1;
        }
        for (idx_t k = max_n + 1; k <= n; ++k) {
            /* log2 values of the two candidates  */
            double log2_2 = (i_exp[i_ptr] + 1) + j_exp[i_ptr] * LOG2_3;
            double log2_3v = i_exp[j_ptr] + (j_exp[j_ptr] + 1) * LOG2_3;

            /* choose the smaller candidate */
            if (log2_2 < log2_3v - 1e-12) {
                i_exp[k] = i_exp[i_ptr] + 1;
                j_exp[k] = j_exp[i_ptr];
                ++i_ptr;
            } else if (log2_3v < log2_2 - 1e-12) {
                i_exp[k] = i_exp[j_ptr];
                j_exp[k] = j_exp[j_ptr] + 1;
                ++j_ptr;
            } else {  /* equal within numerical tolerance */
                i_exp[k] = i_exp[i_ptr] + 1;
                j_exp[k] = j_exp[i_ptr];
                ++i_ptr;
                ++j_ptr;
            }
        }
        max_n = n;
    }

    /* Determine greatest prime factor */
    if (n == 1) return (seq_t)1;
    return (j_exp[n] == 0 ? (seq_t)2 : (seq_t)3);
}
