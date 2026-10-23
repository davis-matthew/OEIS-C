/*
 * A023774  Metadromes: numbers whose digits in base 5 are in strict ascending
 *          order.
 * https://oeis.org/A023774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   numbers whose base‑5 representation consists of strictly
 *            increasing digits (Metadromes, OEIS A023774)
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Comparator for seq_t values, used by qsort. */
static int cmp_seq(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Recursive helper that appends digits larger than 'last' to the
   current number 'cur', storing each new number in 'arr'. */
static void rec(const seq_t cur, const seq_t last, seq_t *arr, size_t *cnt)
{
    for (seq_t d = last + 1; d <= 4; ++d) {
        seq_t new_cur = cur * 5 + d;
        arr[(*cnt)++] = new_cur;
        rec(new_cur, d, arr, cnt);
    }
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    /* The sequence has at most 16 non‑zero terms. */
    static const size_t MAX_NUM = 16;
    seq_t arr[MAX_NUM];
    size_t count = 0;

    /* Single‑digit numbers, including zero. */
    arr[count++] = 0;                 /* a(1) */
    for (seq_t d = 1; d <= 4; ++d) {
        arr[count++] = d;             /* a(2)..a(5) */
        rec(d, d, arr, &count);       /* add all longer numbers starting with d */
    }

    /* Sort the generated numbers to obtain the sequence in increasing order. */
    qsort(arr, count, sizeof(seq_t), cmp_seq);

    /* Indexing starts at 1 as in OEIS. */
    if (n == 0 || n > count) {
        return 0;  /* out‑of‑range, unlikely in official tests */
    }
    return arr[n - 1];
}
