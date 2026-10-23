/*
 * A119590  a(n) = position of n in the lexicographical ordering A119589 of
 *          natural numbers from 1 to 100.
 * https://oeis.org/A119590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = position of n in the lexicographic order of the decimal
 *            strings "1".."100". This implementation builds the permutation
 *            once and returns the pre‑computed rank
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Static tables: 100 entries for the natural numbers 1‑100. */
static seq_t perm[100];
static seq_t rank_arr[101];           /* rank_arr[0] unused */
static bool initialized = false;

/* Convert a small integer (≤ 100) to a NUL‑terminated decimal string. */
static void int_to_str(idx_t x, char *buf)
{
    char tmp[4];
    int len = 0;
    if (x == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    while (x > 0) {
        tmp[len++] = '0' + (x % 10);
        x /= 10;
    }
    for (int i = 0; i < len; ++i)
        buf[i] = tmp[len - 1 - i];
    buf[len] = '\0';
}

/* Comparator for qsort that orders numbers by the lexicographic
   order of their decimal string representations. */
static int cmp_lex(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    idx_t ia = (idx_t)(*va);
    idx_t ib = (idx_t)(*vb);
    char sa[4], sb[4];
    int_to_str(ia, sa);
    int_to_str(ib, sb);
    return strcmp(sa, sb);
}

/* One‑time initialization: create the lexicographic permutation and
   the inverse rank array. */
static void init(void)
{
    if (initialized)
        return;

    /* build the permutation 1..100 */
    for (idx_t i = 0; i < 100; ++i)
        perm[i] = (seq_t)(i + 1);

    /* sort it using qsort (small enough for O(n log n) cost). */
    qsort(perm, 100, sizeof(seq_t), cmp_lex);

    /* compute the inverse rank array. */
    for (idx_t i = 0; i < 100; ++i) {
        idx_t val = (idx_t)(perm[i]);
        rank_arr[val] = (seq_t)(i + 1);   /* positions are 1‑based */
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 1 || n > 100)
        return 0;          /* undefined outside the domain */

    if (!initialized)
        init();

    return rank_arr[(size_t)n];
}
