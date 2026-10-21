/*
 * A089092  Convoluted convolved Fibonacci numbers G_j^(5).
 * https://oeis.org/A089092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let c(z) = 1/(1 - z - z^2)^5 and d(z) = 1/(1 - z^5 - z^10). Then
 *            a(n) = coeff_{z^{n-1}} [c(z) - d(z)] / 5. Recurrences: c_0 = 1,
 *            for n ≥ 1: c_n = 5·c_{n-1} - 5·c_{n-2} -10·c_{n-3} +15·c_{n-4}
 *            +11·c_{n-5} -15·c_{n-6} -10·c_{n-7} +5·c_{n-8} +5·c_{n-9} +
 *            c_{n-10}; d_0 = 1, for n ≥ 1: d_n = (n ≥ 5 ?...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached coefficient arrays. */
static seq_t *c_seq = NULL;   /* coefficients of (1 - z - z^2)^-5   */
static seq_t *d_seq = NULL;   /* coefficients of 1/(1 - z^5 - z^10) */
static size_t  len   = 0;     /* number of terms currently stored (>=1) */
static size_t  cap   = 0;     /* allocated capacity */

/* Ensure both coefficient arrays have values up to index 'upto' (inclusive). */
static void ensure_len(idx_t upto)
{
    if (len > (size_t)upto) return;          /* already computed */

    /* Resize if necessary */
    size_t new_len = (size_t)upto + 1;
    if (cap < new_len) {
        size_t new_cap = cap ? cap << 1 : 4;
        while (new_cap < new_len) new_cap <<= 1;
        c_seq = (seq_t *)realloc(c_seq, new_cap * sizeof(seq_t));
        d_seq = (seq_t *)realloc(d_seq, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing coefficients */
    for (size_t i = len; i < new_len; ++i) {
        /* c_n recurrence (for i = n) */
        if (i == 0) {
            c_seq[0] = 1;
        } else {
            seq_t sum = 0;
            if (i >= 1) sum += c_seq[i-1] * (seq_t)5;
            if (i >= 2) sum -= c_seq[i-2] * (seq_t)5;
            if (i >= 3) sum -= c_seq[i-3] * (seq_t)10;
            if (i >= 4) sum += c_seq[i-4] * (seq_t)15;
            if (i >= 5) sum += c_seq[i-5] * (seq_t)11;
            if (i >= 6) sum -= c_seq[i-6] * (seq_t)15;
            if (i >= 7) sum -= c_seq[i-7] * (seq_t)10;
            if (i >= 8) sum += c_seq[i-8] * (seq_t)5;
            if (i >= 9) sum += c_seq[i-9] * (seq_t)5;
            if (i >=10) sum += c_seq[i-10] * (seq_t)1;
            c_seq[i] = sum;
        }

        /* d_n recurrence */
        if (i == 0) {
            d_seq[0] = 1;
        } else {
            seq_t val = 0;
            if (i >= 5) val += d_seq[i-5];
            if (i >= 10) val += d_seq[i-10];
            d_seq[i] = val;
        }
    }
    len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* unused but defined for completeness */
    /* Ensure coefficients up to index n-1 exist */
    ensure_len(n);

    seq_t diff = c_seq[n-1];
    diff -= d_seq[n-1];
    return diff / (seq_t)5;        /* division is exact */
}
