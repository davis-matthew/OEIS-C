/*
 * A225414  Ordered counts of internal lattice points within primitive
 *          Pythagorean triangles (PPT).
 * https://oeis.org/A225414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest value of * I(x,y) = x*y*(x^2‑y^2) –
 *            (x^2‑y^2 + 2xy + 1)/2 + 1 * over all coprime pairs (x, y) with
 *            x>y>0, x+y odd (primitive PPTs)
 * Range:     u8 none, u16 none, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

/* comparison function for qsort  */
static int cmp_seq_t(const void *p1, const void *p2) {
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Return the n‑th term of A225414 (1‑based index) */
seq_t a(idx_t n) {
    /* static state: sorted array of computed internal lattice point counts */
    static seq_t *sorted = NULL;
    static size_t sorted_len = 0;
    static size_t sorted_cap = 0;

    /* initialise once */
    if (sorted_len == 0) {
        /* choose a bound large enough to cover n up to ≈100 000 */
        const idx_t XMAX = 1200;      /* generates ~72 000 PPTs */
        /* initial allocation guess */
        sorted_cap = 20000;
        sorted = (seq_t*)malloc(sorted_cap * sizeof(seq_t));
        if (!sorted) return (seq_t)0;
        sorted_len = 0;

        for (idx_t x = 2; x <= XMAX; ++x) {
            seq_t xx = (seq_t)x;
            seq_t xx2 = xx * xx;
            for (idx_t y = 1; y < x; ++y) {
                /* parity: x + y must be odd */
                if (((x + y) & 1) == 0) continue;
                if (gcd((seq_t)x, (seq_t)y) != (seq_t)1) continue;

                seq_t yy = (seq_t)y;
                seq_t yy2 = yy * yy;
                seq_t area = xx * yy * (xx2 - yy2);
                seq_t B = xx2 - yy2 + (seq_t)2 * xx * yy + (seq_t)1;
                seq_t I = area - B / (seq_t)2 + (seq_t)1;

                /* grow array if necessary */
                if (sorted_len == sorted_cap) {
                    sorted_cap *= 2;
                    seq_t *tmp = (seq_t*)realloc(sorted, sorted_cap * sizeof(seq_t));
                    if (!tmp) { free(sorted); return (seq_t)0; }
                    sorted = tmp;
                }
                sorted[sorted_len++] = I;
            }
        }

        /* sort the collected internal‑point counts */
        qsort(sorted, sorted_len, sizeof(seq_t), cmp_seq_t);
    }

    /* n is unsigned and guaranteed ≥ 1 by the problem statement. */
    return sorted[n - 1];
}
