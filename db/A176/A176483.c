/*
 * A176483  Triangle, read by rows, defined by T(n, k) = b(n) - b(k) - b(n-k)
 *          + 1, where b(n) = 5*b(n-1) - 4*b(n-2) + 3*b(n-3) - 2*b(n-4) -
 *          b(n-5) and b(0) = 0, b(1) = 1, b(2) = 5, b(3) = 21, b(4) = 88.
 * https://oeis.org/A176483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) read row‑wise from the triangle T(n,k)=b(n)-b(k)-b(n-k)+1,
 *            where the number sequence b(n) satisfies
 *            b(n)=5·b(n‑1)-4·b(n‑2)+3·b(n‑3)-2·b(n‑4)-b(n‑5) with initial
 *            values b(0)=0, b(1)=1, b(2)=5, b(3)=21, b(4)=88
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 153, u64 n <= 529,
 *            u128 n <= 2017, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static storage for the b(n) sequence */
static seq_t *b_values = NULL;   /* array of computed b(n) values   */
static idx_t  b_max   = 4;       /* maximum index that has been computed */
static size_t b_size  = 0;       /* allocated array size */

/* Ensure that b_values[0..n] are defined.  The array grows as needed. */
static void ensure_b(idx_t n)
{
    /* Allocate initial array if not yet allocated */
    if (b_values == NULL) {
        b_size = 5;                                 /* indices 0..4 are known */
        b_values = (seq_t *)malloc(b_size * sizeof(seq_t));
        if (!b_values) return;  /* allocation failure – caller will tolerate */
        b_values[0] = 0;
        b_values[1] = 1;
        b_values[2] = 5;
        b_values[3] = 21;
        b_values[4] = 88;
    }

    /* Return immediately if the requested range is already computed */
    if (n <= b_max) return;

    /* Grow the array if necessary */
    if ((size_t)(n + 1) > b_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(b_values, new_size * sizeof(seq_t));
        if (!tmp) return;  /* allocation failure – caller will tolerate */
        b_values = tmp;
        b_size = new_size;
    }

    /* Compute the missing values using the recurrence */
    for (idx_t i = b_max + 1; i <= n; ++i) {
        /* b(i) = 5*b(i-1) - 4*b(i-2) + 3*b(i-3) - 2*b(i-4) - b(i-5) */
        seq_t p = b_values[i - 1] * 5;
        seq_t q = b_values[i - 2] * 4;
        seq_t r = b_values[i - 3] * 3;
        seq_t s = b_values[i - 4] * 2;
        seq_t t = b_values[i - 5];
        b_values[i] = p - q + r - s - t;
    }
    b_max = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    for (;; ++r) {
        idx_t off = r * (r + 1) / 2;
        if (off > n) { r--; break; }
    }

    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;               /* position within the row   */

    /* Ensure that b_values[0..r] are available */
    ensure_b(r);

    /* Compute T(r,k) = b(r) - b(k) - b(r-k) + 1 */
    seq_t val = b_values[r] - b_values[k] - b_values[r - k] + 1;
    return val;
}
