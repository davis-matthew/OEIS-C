/*
 * A045000  Numbers whose base-3 representation contains exactly two 0's and
 *          one 2.
 * https://oeis.org/A045000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 754, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers whose base‑3 representation contains exactly
 * two 0's and one 2, in ascending order.  The sequence starts
 * with 18, 29, 33, … as in OEIS A045000. */
#include <stdlib.h>

/* Comparator for qsort used on seq_t objects. */
static int seq_compare(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Generate all valid numbers of a given ternary length `len` and
 * append them (sorted) to the global buffer `arr`.  `arr_len` is
 * updated to reflect the new length of the buffer.  A local array
 * is used to hold the generated values before sorting. */
static void generate_length(idx_t len, seq_t *arr, size_t *arr_len)
{
    /* Total unordered combinations of three distinct positions. */
    size_t m = len * (len - 1) * (len - 2) / 2;

    /* Allocate a temporary buffer for the generated values. */
#ifdef __cplusplus
    seq_t *local = new seq_t[m];
#else
    seq_t *local = (seq_t *)malloc(m * sizeof(seq_t));
    if (!local) return;      /* allocation failure – should not happen */
#endif

    size_t local_len = 0;

    /* Enumerate all choices of two zeros and one two. */
    for (idx_t pos0 = 0; pos0 < len; ++pos0) {
        for (idx_t pos1 = pos0 + 1; pos1 < len; ++pos1) {
            /* Zeros cannot be in the leading position. */
            if (pos0 == 0 || pos1 == 0) continue;
            for (idx_t pos2 = 0; pos2 < len; ++pos2) {
                if (pos2 == pos0 || pos2 == pos1) continue;

                /* Compute the decimal value of the base‑3 number. */
                seq_t val = 0;
                for (idx_t p = 0; p < len; ++p) {
                    seq_t digit;
                    if (p == pos0 || p == pos1)      digit = 0;
                    else if (p == pos2)             digit = 2;
                    else                            digit = 1;
                    val = val * 3 + digit;
                }
                local[local_len++] = val;
            }
        }
    }

    /* Sort the numbers of this length. */
    qsort(local, local_len, sizeof(seq_t), seq_compare);

    /* Append them to the global buffer. */
    for (size_t i = 0; i < local_len; ++i)
        arr[*arr_len + i] = local[i];
    *arr_len += local_len;

    /* Free the temporary buffer. */
#ifdef __cplusplus
    delete[] local;
#else
    free(local);
#endif
}

/* Global buffer that stores the precomputed sequence values. */
static seq_t        seq_buf[50000];   /* capacity is safe for n ≤ 10000 */
static size_t       seq_len = 0;      /* number of values stored */
static idx_t        current_len = 2;  /* last processed ternary length */

/* External function required by the framework.  It returns the
 * n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n=0, harmless */

    /* Generate terms until we have at least n values. */
    while (seq_len < (size_t)n) {
        ++current_len;                       /* next ternary length to process */
        generate_length(current_len, seq_buf, &seq_len);
    }
    return seq_buf[(size_t)(n - 1)];
}
