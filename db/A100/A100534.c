/*
 * A100534  Number of partitions of 2*n into parts of two kinds.
 * https://oeis.org/A100534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000712(2*n) = number of partitions of (2*n) into parts
 *            of two kinds. A000712(m) = sum_{k=0}^{m} p(k) * p(m-k) where p()
 *            is the ordinary partition function
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 33, u64 n <= 108, u128 n <= 374,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* List all needed indices and partition numbers up to the highest 2*n requested.
   Because the function may be called with arbitrary n order, we cache the
   partition array using static variables.  The recurrence used is the
   Pentagonal‑number theorem.  It requires signed arithmetic; the build
   using C++ will provide an arbitrary‑precision signed type (e.g. boost::multiprecision::cpp_int).
   For builds where seq_t is unsigned big integer the expression below still works
   because the recurrence never requires a negative intermediate result
   (pos > neg for all n).  On such platforms the compiler automatically
   interprets the signed subtraction correctly. */

static seq_t *p_arr = NULL;   /* p_arr[i] stores p(i) */
static idx_t  p_len = 0;      /* number of valid entries : last index+1 */

/* Ensure p_arr contains partition numbers up to max_n (inclusive). */
static void ensure_partitions(idx_t max_n)
{
    if (p_len > max_n + 1) return;               /* already computed */

    /* Grow the array. */
    size_t new_len = (size_t)max_n + 1;
    seq_t *tmp = (seq_t *)realloc(p_arr, new_len * sizeof(seq_t));
    if (!tmp) return;  /* allocation failed; caller will see all entries missing (undefined) */
    p_arr = tmp;

    /* Initialize if first time. */
    if (p_len == 0) {
        p_arr[0] = 1;   /* p(0)=1 */
        p_len   = 1;
    }

    /* Compute missing partition numbers. */
    for (idx_t n = p_len; n <= max_n; ++n) {
        seq_t sum = 0;
        for (int k = 1; ; ++k) {
            size_t g1 = k * (3 * k - 1) / 2;
            if (g1 > (size_t)n) break;
            if (k & 1) sum += p_arr[n - g1];
            else      sum -= p_arr[n - g1];

            size_t g2 = k * (3 * k + 1) / 2;
            if (g2 > (size_t)n) continue;
            if (k & 1) sum += p_arr[n - g2];
            else      sum -= p_arr[n - g2];
        }
        p_arr[n] = sum;
    }
    p_len = new_len;
}

seq_t a(idx_t n)
{
    /* We need partitions up to 2*n (inclusive). */
    idx_t target = n * 2;
    ensure_partitions(target);
    if (p_arr == NULL) return 0;   /* allocation failure */

    /* Compute a(n) = sum_{k=0}^{2n} p(k)*p(2n-k)  (convolution). */
    idx_t m = target;
    seq_t result = 0;
    for (idx_t k = 0; k <= m / 2; ++k) {
        seq_t prod = p_arr[k] * p_arr[m - k];
        if (k < m - k) {
            result += prod;
            result += prod;   /* symmetric term */
        } else {
            result += prod;   /* middle term when m even */
        }
    }
    return result;
}
