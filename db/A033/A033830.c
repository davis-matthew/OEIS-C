/*
 * A033830  Numbers that can be expressed as the product of three 2-digit
 *          numbers.
 * https://oeis.org/A033830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   The n-th term is the n-th smallest number that can be expressed
 *            as the product of three 2-digit integers (10…99)
 * Range:     u8 none, u16 none, u32 n <= 8587, u64 n <= 8587, u128 n <= 8587,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of the function */
static int compare_seq(const void *, const void *);

/* Global cache for the sequence */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;

/* Comparator for qsort that works with any seq_t size */
static int compare_seq(const void *p1, const void *p2)
{
    const seq_t a = *(const seq_t *)p1;
    const seq_t b = *(const seq_t *)p2;
    return (a > b) ? 1 : (a < b) ? -1 : 0;
}

/* Build the entire sequence on first call */
static void build_sequence(void)
{
    if (seq_cache != NULL)   /* already built */
        return;

    /* Number of possible triples (a,b,c) with 10<=a,b,c<=99 */
    const size_t total = 90u * 90u * 90u;  /* 729000 */

    /* Allocate raw buffer for all products */
    seq_t *buf = (seq_t *)malloc(total * sizeof(seq_t));
    if (buf == NULL)
        return; /* allocation failure; caller may receive 0 for all entries */

    size_t count = 0;
    for (idx_t a = 10; a <= 99; ++a) {
        for (idx_t b = 10; b <= 99; ++b) {
            for (idx_t c = 10; c <= 99; ++c) {
                buf[count++] = (seq_t)a * (seq_t)b * (seq_t)c;
            }
        }
    }

    /* Sort all products */
    qsort(buf, count, sizeof(seq_t), compare_seq);

    /* Deduplicate in place */
    size_t uniq = 0;
    for (size_t i = 0; i < count; ++i) {
        if (i == 0 || buf[i] != buf[i - 1]) {
            buf[uniq++] = buf[i];
        }
    }

    /* Reallocate to exact size (optional) */
    seq_t *seq = (seq_t *)realloc(buf, uniq * sizeof(seq_t));
    if (seq == NULL) /* realloc failed, keep original buffer */
        seq = buf;

    seq_cache = seq;
    seq_len = uniq;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* n is 1-based */
    if (n == 0)  /* invalid index, return 0 */
        return 0;

    build_sequence();
    if (seq_cache == NULL)
        return 0;  /* failed to build sequence */

    /* Convert n to zero–based index safely */
    size_t idx = (size_t)(n - (idx_t)1);
    if (idx >= seq_len)
        return 0;  /* out of bounds */

    return seq_cache[idx];
}
