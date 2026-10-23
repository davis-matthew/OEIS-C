/*
 * A172347  Triangle t(n,k) read by rows: fibonomial ratios c(n)/(c(k)*c(n-k))
 *          where c are partial products of a generalized Fibonacci sequence
 *          with multiplier m=3.
 * https://oeis.org/A172347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = t(r,k) where t(r,k) = c(r)/(c(k)*c(r-k)); * Here c(n) is
 *            the partial product of the generalized Fibonacci sequence * with
 *            multiplier 3: c(0)=1, c(n)=c(n-1)*F(n) for n≥1, * with
 *            F(1)=F(2)=1 and F(n)=F(n-1)+3*F(n-2)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 45, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* static storage for partial products c(n) */
static seq_t *carr = NULL;
static idx_t size_c = 0;
static size_t cap = 0;

/* current Fibonacci values for generating further terms */
static seq_t f_prev1 = 0; /* F(i)   */
static seq_t f_prev2 = 0; /* F(i-1) */

/* Ensure carr contains partial products up to up_to (inclusive) */
static void ensure_c(idx_t up_to)
{
    if (carr == NULL) {
        cap = 4;
        carr = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!carr) return;                /* allocation failed; caller will fail gracefully */
        carr[0] = (seq_t)1;               /* c(0) = 1 */
        size_c = 1;
        f_prev1 = (seq_t)1;               /* F(2) */
        f_prev2 = (seq_t)1;               /* F(1) */
    }

    while (size_c <= up_to) {
        if (size_c >= cap) {
            cap *= 2;
            seq_t *tmp = (seq_t *)realloc(carr, cap * sizeof(seq_t));
            if (!tmp) return;            /* allocation failed */
            carr = tmp;
        }

        if (size_c == 1) {
            /* c(1) = c(0) * F(1) */
            seq_t newc = carr[0] * f_prev2;
            carr[size_c++] = newc;
        } else if (size_c == 2) {
            /* c(2) = c(1) * F(2) */
            seq_t newc = carr[1] * f_prev1;
            carr[size_c++] = newc;
        } else {
            /* Compute next Fibonacci number: F(i+1) = F(i) + 3*F(i-1) */
            seq_t f_next = f_prev1 + (f_prev2 * (seq_t)3);
            seq_t newc = carr[size_c - 1] * f_next;
            carr[size_c++] = newc;
            /* Advance Fibonacci history */
            f_prev2 = f_prev1;
            f_prev1 = f_next;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine row r and column k from flattened index n */
    idx_t r = 0;
    idx_t offset = 0;
    while (offset + r + 1 <= n) {
        r++;
        offset += r;
    }
    idx_t k = n - offset;

    /* Compute partial products up to row r */
    ensure_c(r);

    if (k == 0 || k == r)
        return (seq_t)1;

    seq_t denom = carr[k] * carr[r - k];
    return carr[r] / denom;
}
