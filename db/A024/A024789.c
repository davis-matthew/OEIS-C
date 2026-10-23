/*
 * A024789  Number of 5's in all partitions of n.
 * https://oeis.org/A024789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5’s in all partitions of n (OEIS A024789)
 * Range:     u8 n <= 20, u16 n <= 45, u32 n <= 125, u64 n <= 400,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *p_arr = NULL;      /* p[n] : partition numbers */
static seq_t *a_arr = NULL;      /* a[n] : number of 5’s in all partitions of n */
static idx_t  max_n = 0;         /* largest n computed so far */

/* Allocate and compute p[n] and a[n] for all n ≤ new_n. */
static void ensure_arrays(idx_t new_n)
{
    if (new_n <= max_n) return;          /* already computed */

    size_t new_size = (size_t)(new_n + 1);

    seq_t *p_new = NULL;
    seq_t *a_new = NULL;
#ifdef __cplusplus
    p_new = new seq_t[new_size];
    a_new = new seq_t[new_size];
#else
    p_new = (seq_t*)malloc(new_size * sizeof(seq_t));
    a_new = (seq_t*)malloc(new_size * sizeof(seq_t));
#endif

    /* initialize */
    for (idx_t i = 0; i <= new_n; ++i) {
        p_new[i] = 0;
        a_new[i] = 0;
    }
    p_new[0] = (seq_t)1;                 /* p[0] = 1 */

    /* partition numbers: p[j] += p[j-k] for each part size k */
    for (idx_t k = 1; k <= new_n; ++k) {
        for (idx_t j = k; j <= new_n; ++j) {
            p_new[j] += p_new[j - k];
        }
    }

    /* a[n] via recurrence a[n] = a[n-5] + p[n-5] for n ≥ 5 */
    for (idx_t i = 5; i <= new_n; ++i) {
        a_new[i] = a_new[i - 5] + p_new[i - 5];
    }

    /* free old arrays */
    if (p_arr) {
#ifdef __cplusplus
        delete[] p_arr;
        delete[] a_arr;
#else
        free(p_arr);
        free(a_arr);
#endif
    }

    p_arr = p_new;
    a_arr = a_new;
    max_n = new_n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for n = 0, though offset is 1 */
    ensure_arrays(n);
    return a_arr[n];
}
