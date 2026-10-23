/*
 * A190016  Numbers 1 through 10000 sorted lexicographically in decimal
 *          representation.
 * https://oeis.org/A190016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the n-th lexicographically sorted integer from 1 to 10000
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_array, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_to_str.h"
#include "../utils/seq_alloc_array.h"

/* Compare two seq_t values lexicographically by their decimal string representation */
static int cmp_seq_lex(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    char buf1[32], buf2[32];
    size_t len1, len2;

    seq_to_str(a, buf1, &len1);
    seq_to_str(b, buf2, &len2);

    buf1[len1] = '\0';
    buf2[len2] = '\0';

    return strcmp(buf1, buf2);
}

seq_t a(idx_t n)
{
    static seq_t *sorted = NULL;
    const idx_t N = 10000;

    if (!sorted) {
        /* Allocate and fill the array with 1..10000 */
        sorted = seq_alloc_array(N);
        for (idx_t i = 0; i < N; ++i) {
            sorted[i] = (seq_t)(i + 1);
        }
        /* Sort lexicographically */
        qsort(sorted, N, sizeof(seq_t), cmp_seq_lex);
    }

    if (n == 0 || n > N)
        return 0;        /* undefined for out‑of‑range indices */

    return sorted[n - 1];
}
