/*
 * A174068  Convolved with its aerated variant of two zeros between terms =
 *          A000041.
 * https://oeis.org/A174068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the convolution relation A(x)·A(x^3) = P(x),
 *            where P(x) is the partition generating function.
 *            Coefficientwise: p(n) = a(n) + Σ_{k=1..⌊n/3⌋} a(k)*a(n‑3k), thus
 *            a(0)=1, a(n)=p(n) – Σ_{k=1..⌊n/3⌋} a(k)*a(n‑3k) (n>0). Here p(n)
 *            is the partition number (OEIS A000041). The implementa...
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 163, u64 n <= 540,
 *            u128 n <= 1911, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Grow a dynamic array of seq_t elements to hold at least 'needed' entries. */
static void
ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed)
        return;
    size_t new_cap = (*cap == 0) ? needed + 1 : (*cap << 1);
    if (new_cap < needed + 1)
        new_cap = needed + 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!*arr) abort();
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Static tables to hold partitions p[] and the desired sequence a[]. */
    static seq_t *p_arr = NULL;
    static size_t p_cap = 0;
    static idx_t p_last = (idx_t)(-1);          /* -1 == UINT64_MAX */

    static seq_t *a_arr = NULL;
    static size_t a_cap = 0;
    static idx_t a_last = (idx_t)(-1);

    /* Ensure arrays are large enough. */
    ensure_capacity(&p_arr, &p_cap, n + 1);
    ensure_capacity(&a_arr, &a_cap, n + 1);

    /* Compute partition numbers p[0..n] if needed. */
    if (p_last == (idx_t)(-1)) {
        p_arr[0] = 1;   /* p(0) = 1 */
        p_last = 0;
    }
    for (idx_t i = p_last + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t k = 1;; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i) break;
            if (k & 1)
                pos += p_arr[i - g1];
            else
                neg += p_arr[i - g1];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= i) {
                if (k & 1)
                    pos += p_arr[i - g2];
                else
                    neg += p_arr[i - g2];
            }
        }
        p_arr[i] = pos - neg;   /* pos ≥ neg for all i, no underflow */
        p_last = i;
    }

    /* Compute a[0..n] if needed. */
    if (a_last == (idx_t)(-1)) {
        a_arr[0] = 1;   /* a(0) = 1 */
        a_last = 0;
    }
    for (idx_t i = a_last + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; 3 * k <= i; ++k) {
            sum += a_arr[k] * a_arr[i - 3 * k];
        }
        a_arr[i] = p_arr[i] - sum;
        a_last = i;
    }

    return a_arr[n];
}
