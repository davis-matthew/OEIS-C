/*
 * A106407  Expansion of x((1-x)(1-x^2)(1-x^4)(1-x^8)...)^2.
 * https://oeis.org/A106407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) is the self–convolution of the signed Thue–Morse sequence
 *            A106400; that is a(n) = Σ_{i=0}^{n-1} s(i) · s(n-1-i), where
 *            s(i) = (-1)^{popcount(i)}. a(1)=1. The implementation caches
 *            already computed values in a table so that subsequent calls are
 *            O(1). Convolution is computed in O(n^2) time for...
 * Range:     i8 n <= 84, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#if defined(__cplusplus)
# include <new>
#endif

/* Return the signed Thue–Morse value for index i.
   i is an unsigned 64‑bit integer. */
static inline seq_t signed_thue(idx_t i)
{
    return ((__builtin_popcountll(i) & 1U) ? (seq_t)-1 : (seq_t)1);
}

/* Cached table of a(n) values.  a_arr[0] is unused;  a_arr[n] holds a(n). */
static seq_t *a_arr = NULL;   /* allocated array of seq_t elements */
static idx_t  a_size = 0;     /* largest n for which a_arr[n] is valid */

/* Allocate or enlarge the a_arr table to hold at least indices 0..need. */
static void ensure_a(idx_t need)
{
    if (need <= a_size)
        return;                     /* already enough */

    /* Allocate a fresh table of size need+1 */
    seq_t *tmp;
#if defined(__cplusplus)
    tmp = new seq_t[need + 1];
#else
    tmp = (seq_t *)malloc((size_t)(need + 1) * sizeof(seq_t));
#endif
    if (!tmp && need != 0)               /* allocation failure – abort */
        abort();

    /* Copy existing entries (if any) */
    if (a_arr) {
        for (idx_t i = 0; i <= a_size; ++i)
            tmp[i] = a_arr[i];
#if defined(__cplusplus)
        delete[] a_arr;
#else
        free(a_arr);
#endif
    }

    /* Initialize new entries.  a_arr[0] is unused – set to 0. */
    if (a_size == 0)
        tmp[0] = (seq_t)0;

    /* Compute new a(n) values.  Each a(k) = Σ_{i=0}^{k-1} s(i)·s(k-1-i). */
    for (idx_t k = a_size + 1; k <= need; ++k) {
        seq_t sum = (seq_t)0;
        for (idx_t i = 0; i < k; ++i) {
            sum += signed_thue(i) * signed_thue(k - 1 - i);
        }
        tmp[k] = sum;
    }

    a_arr = tmp;
    a_size = need;
}

/* Public function requested by the assignment. */
seq_t a(idx_t n)
{
    if (n == 0)   /* not defined in OEIS – return 0 safely */
        return (seq_t)0;

    ensure_a(n);
    return a_arr[n];
}
