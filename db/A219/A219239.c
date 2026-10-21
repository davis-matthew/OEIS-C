/*
 * A219239  Double magic numbers (in physics).
 * https://oeis.org/A219239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) is the nth smallest positive integer that can be expressed
 *            as the sum of two (not necessarily distinct) numbers from the
 *            set S = {2, 8, 20, 28, 50, 82, 126}
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const int MAGIC[] = {2, 8, 20, 28, 50, 82, 126};
#define MAGIC_CNT 7
#define SUM_CNT   28

/* Build the sorted list of all 28 distinct sums x+y with x,y in MAGIC. */
static void build_sums(seq_t *out)
{
    size_t k = 0;
    for (size_t i = 0; i < MAGIC_CNT; ++i) {
        for (size_t j = i; j < MAGIC_CNT; ++j) {
            out[k++] = (seq_t)MAGIC[i] + (seq_t)MAGIC[j];
        }
    }
    /* Simple insertion sort – the array size is only 28. */
    for (size_t i = 1; i < SUM_CNT; ++i) {
        seq_t key = out[i];
        size_t j = i;
        while (j > 0 && out[j - 1] > key) {
            out[j] = out[j - 1];
            --j;
        }
        out[j] = key;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return 0;                 /* undefined index */
    static seq_t cache[SUM_CNT] = {0};
    static int initialized = 0;
    if (!initialized) {
        build_sums(cache);
        initialized = 1;
    }
    if ((idx_t)(n - 1) < (idx_t)SUM_CNT)
        return cache[(size_t)(n - 1)];
    return 0;   /* out of range – sequence is finite */
}
