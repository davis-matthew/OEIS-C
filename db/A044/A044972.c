/*
 * A044972  Numbers whose base-3 representation contains exactly one 0 and two
 *          1's.
 * https://oeis.org/A044972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the n‑th number whose base‑3 representation contains
 *            exactly one digit 0 and exactly two digits 1 (OEIS A044972)
 * Range:     u8 n <= 35, u16 n <= 870, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define TMP_SIZE 5000   /* fits all counts needed for n ≤ ~10 000 */

static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for the sequence, keep 0 */

    /* Find the base‑3 length L that contains the n‑th term. */
    idx_t cumulative = 0;
    idx_t L = 3;
    for (;; ++L) {
        idx_t cnt = (L - 1) * (L - 1) * (L - 2) / 2;  /* patterns for length L */
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
    }

    idx_t cnt = (L - 1) * (L - 1) * (L - 2) / 2;  /* patterns for length L */
    idx_t k   = n - cumulative - 1;                 /* 0‑based offset in this block */

    if (cnt > TMP_SIZE) abort();          /* safety guard */

    seq_t block[TMP_SIZE];
    idx_t pos = 0;
    unsigned char digits[64];              /* L never exceeds 64 for our n range */

    /* Enumerate all patterns of length L. */
    for (idx_t zero = 1; zero < L; ++zero) {          /* zero cannot be the first digit */
        for (idx_t o1 = 0; o1 < L; ++o1) {
            if (o1 == zero) continue;
            for (idx_t o2 = o1 + 1; o2 < L; ++o2) {
                if (o2 == zero) continue;

                for (idx_t i = 0; i < L; ++i) digits[i] = 2;
                digits[zero] = 0;
                digits[o1]   = 1;
                digits[o2]   = 1;

                seq_t val = 0;
                for (idx_t i = 0; i < L; ++i)
                    val = val * 3 + (seq_t)digits[i];

                block[pos++] = val;
            }
        }
    }

    qsort(block, cnt, sizeof(seq_t), cmp_seq_t);
    return block[k];
}
