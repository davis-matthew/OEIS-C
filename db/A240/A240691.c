/*
 * A240691  Number of partitions p of n such that (# 1s in p) = (#1s in
 *          conjugate(p)).
 * https://oeis.org/A240691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(n+1) - b(n), b(n+1) = p(n) - b(n), where p(n) is the
 *            ordinary partition number. This follows from the identities:
 *            a(n) = A240690(n+1) - A240690(n), a(n) + 2*A240690(n) = p(n)
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *p_arr = NULL;   /* ordinary partitions p(n), 0 <= n */
static seq_t *b_arr = NULL;   /* A240690(n), 1 <= n */
static seq_t *a_arr = NULL;   /* desired sequence a(n), 1 <= n */
static idx_t  computed = 0;   /* largest n processed (a_arr[computed] valid) */
static size_t array_size = 0; /* allocated size of the arrays */

/* Ensure arrays are large enough and all values up to n are computed. */
static void ensure_up_to(idx_t n)
{
    /* Allocate or grow the arrays if necessary */
    if (p_arr == NULL) {
        array_size = (size_t)n + 3;                     /* guard space for b[n+1] */
        p_arr = (seq_t *)malloc(array_size * sizeof(seq_t));
        b_arr = (seq_t *)malloc(array_size * sizeof(seq_t));
        a_arr = (seq_t *)malloc(array_size * sizeof(seq_t));
        for (idx_t i = 0; i < array_size; ++i) {
            p_arr[i] = 0;
            b_arr[i] = 0;
            a_arr[i] = 0;
        }
        p_arr[0] = (seq_t)1;   /* p(0) = 1 */
        b_arr[1] = (seq_t)0;   /* A240690(1) = 0 */
        computed = 0;
    } else if ((size_t)n + 3 > array_size) {
        size_t newsize = (size_t)n + 3;
        p_arr = (seq_t *)realloc(p_arr, newsize * sizeof(seq_t));
        b_arr = (seq_t *)realloc(b_arr, newsize * sizeof(seq_t));
        a_arr = (seq_t *)realloc(a_arr, newsize * sizeof(seq_t));
        for (idx_t i = array_size; i < newsize; ++i) {
            p_arr[i] = 0;
            b_arr[i] = 0;
            a_arr[i] = 0;
        }
        array_size = newsize;
    }

    /* Compute values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* Euler's pentagonal recurrence for p(i) */
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            if (k & 1) pos += p_arr[i - pent1];
            else       neg += p_arr[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            if (k & 1) pos += p_arr[i - pent2];
            else       neg += p_arr[i - pent2];
        }
        p_arr[i] = pos - neg;   /* p(i) = pos - neg, pos >= neg holds */

        /* Compute b[i+1] and a[i] */
        seq_t b_next = p_arr[i] - b_arr[i];   /* b[i+1] = p(i) - b(i) */
        b_arr[i + 1] = b_next;
        a_arr[i] = b_next - b_arr[i];         /* a(i) = b_next - b(i) */
    }
    computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n < 1 */
    ensure_up_to(n);
    return a_arr[n];
}
