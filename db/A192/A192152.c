/*
 * A192152  Monotonic ordering of nonnegative differences 6^i-3^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A192152: monotonic ordering of nonnegative differences 6^i - 3^j
   (0 ≤ i ≤ 40, j ≥ 0).  The function returns the n-th term with OEIS
   offset 1.  The implementation generates all values, sorts them
   ascending, removes duplicates, and then looks up the requested index.
   It works for any unsigned integer type seq_t (byte‑sized up to 64 bits
   or arbitrary precision) and any idx_t type. */
#include <stdlib.h>
#include <stdbool.h>

/* Comparison routine for sorting values in ascending order.  It works
   for basic integer types as well as for arbitrary‑precision types
   that provide operator< > and ==. */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* static state – generates the sequence only once */
    static bool initialized = false;
    static seq_t *values   = NULL;
    static idx_t  uniq_len = 0;  /* number of stored terms   */

    if (!initialized) {
        /* ----------------------------------------------------------------
           Generate all nonnegative differences 6^i – 3^j with 0 ≤ i ≤ 40.
           For builtin integer types we must avoid overflow; for
           arbitrary‑precision types the overflow tests are skipped.       */
        const idx_t I_MAX = 40u;                /* guaranteed upper bound */
        size_t cap = 2048;                      /* provisional capacity  */
        seq_t *tmp = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!tmp) {      /* allocation failure – return zero  */
            return (seq_t)0;
        }

        size_t len = 0;                         /* number of generated values */

        /* current 6^i */
        seq_t pow6 = 1;
        for (idx_t i = 0; ; ++i) {
            /* For builtin types guard against overflow of pow6 * 6 */
            if (sizeof(seq_t) <= 8) {
                /* calculate maximum unsigned value of this type */
                unsigned long long MAX = (sizeof(seq_t) == 1) ? 0xFFULL :
                                         (sizeof(seq_t) == 2) ? 0xFFFFULL :
                                         (sizeof(seq_t) == 4) ? 0xFFFFFFFFULL :
                                         0xFFFFFFFFFFFFFFFFULL;
                if (pow6 > (seq_t)(MAX / 6ULL)) break;  /* further i would overflow */
            }

            /* Enumerate j while 3^j ≤ 6^i */
            seq_t pow3 = 1;
            while (pow3 <= pow6) {
                seq_t diff = pow6 - pow3;        /* nonnegative */

                /* store diff */
                if (len == cap) {
                    cap <<= 1;
                    seq_t *tmp2 = (seq_t *)realloc(tmp, cap * sizeof(seq_t));
                    if (!tmp2) { free(tmp); return (seq_t)0; }
                    tmp = tmp2;
                }
                tmp[len++] = diff;

                /* prepare next 3^j, guard against builtin overflow */
                if (sizeof(seq_t) <= 8) {
                    unsigned long long MAX = (sizeof(seq_t) == 1) ? 0xFFULL :
                                             (sizeof(seq_t) == 2) ? 0xFFFFULL :
                                             (sizeof(seq_t) == 4) ? 0xFFFFFFFFULL :
                                             0xFFFFFFFFFFFFFFFFULL;
                    if (pow3 > (seq_t)(MAX / 3ULL)) break;  /* next multiplication overflows */
                }
                pow3 *= 3;
            }

            /* increment i, stop if we reached I_MAX or if pow6 would overflow */
            if (i == (idx_t)I_MAX) break;
            /* multiply pow6 by 6 for next i */
            if (sizeof(seq_t) <= 8) {
                unsigned long long MAX = (sizeof(seq_t) == 1) ? 0xFFULL :
                                         (sizeof(seq_t) == 2) ? 0xFFFFULL :
                                         (sizeof(seq_t) == 4) ? 0xFFFFFFFFULL :
                                         0xFFFFFFFFFFFFFFFFULL;
                if (pow6 > (seq_t)(MAX / 6ULL)) break;
            }
            pow6 *= 6;
        }

        /* ----------------------------------------------------------------
           Sort the generated values in ascending order.  We use a simple
           insertion sort – the total number of values is small
           (well below 2000), so O(n²) is acceptable for both speed
           and compatibility with arbitrary‑precision types.             */
        for (size_t i = 1; i < len; ++i) {
            seq_t key = tmp[i];
            size_t j = i;
            while (j > 0 && tmp[j - 1] > key) {
                tmp[j] = tmp[j - 1];
                --j;
            }
            tmp[j] = key;
        }

        /* ----------------------------------------------------------------
           Remove duplicates – the array is now contiguous.                */
        size_t ulen = 0;
        for (size_t i = 0; i < len; ++i) {
            if (ulen == 0 || tmp[i] > tmp[ulen - 1]) {
                tmp[ulen++] = tmp[i];
            }
        }

        /* shrink to the exact number of unique values */
        if (ulen < len) {
            seq_t *tmp2 = (seq_t *)realloc(tmp, ulen * sizeof(seq_t));
            if (tmp2) tmp = tmp2;
        }

        /* store the static final array */
        values   = tmp;
        uniq_len = (idx_t)ulen;
        initialized = true;
    }

    /* ----------------------------------------------------------------
       Return the k‑th term (OEIS offset 1).  n == 0 is out of range –
       we return 0.  If n exceeds the precomputed range, we also return 0. */
    if (n == 0) return (seq_t)0;
    idx_t id = n - 1;
    if (id >= uniq_len) return (seq_t)0;
    return values[id];
}
