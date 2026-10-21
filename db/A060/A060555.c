/*
 * A060555  String together the first n numbers in an order which minimizes
 *          the result.
 * https://oeis.org/A060555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = minimal concatenation of 1..n (lexicographically smallest
 *            integer obtained by concatenating the numbers 1,2,…,n in some
 *            order)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Count decimal digits of a uint64_t value.  */
static int digits_of(uint64_t v) {
    int d = 0;
    if (v == 0) return 1;
    while (v) {
        v /= 10;
        ++d;
    }
    return d;
}

/* Compute 10^e as seq_t.  For the 64‑bit build this is a normal integer. */
static seq_t pow10_ui(unsigned int e) {
    seq_t p = (seq_t)1;
    while (e--) p *= (seq_t)10;
    return p;
}

/* Comparator for qsort: a comes before b if a+b < b+a in decimal. */
static int cmp_idx(const void *p1, const void *p2) {
    const idx_t *a = (const idx_t *)p1;
    const idx_t *b = (const idx_t *)p2;
    int da = digits_of((uint64_t)*a);
    int db = digits_of((uint64_t)*b);
    seq_t ab = (seq_t)(*a) * pow10_ui(db) + (seq_t)(*b);
    seq_t ba = (seq_t)(*b) * pow10_ui(da) + (seq_t)(*a);
    if (ab < ba) return -1;
    if (ab > ba) return  1;
    return 0;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* safety: offset is 1 */
    /* Allocate and fill array 1..n */
    idx_t *arr = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!arr) return (seq_t)0;      /* unlikely, abort on allocation failure */
    for (idx_t i = 1; i <= n; ++i) arr[i - 1] = i;
    /* Sort according to the custom comparator */
    qsort(arr, (size_t)n, sizeof(idx_t), cmp_idx);
    /* Concatenate in sorted order */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < n; ++i) {
        idx_t num = arr[i];
        seq_t mult = pow10_ui(digits_of((uint64_t)num));
        result = result * mult + (seq_t)num;
    }
    free(arr);
    return result;
}
