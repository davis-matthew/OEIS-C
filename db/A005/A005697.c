/*
 * A005697  Positions of remoteness 5 in Beans-Don't-Talk.
 * https://oeis.org/A005697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) is the n-th k whose remoteness is 5 in
 *            Beans-Don't-Talk, where a move takes k to the odd part of 3k+1
 *            or 3k-1 and 1 is terminal
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   odd_part_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/odd_part_u64.h"

/* exact remoteness of m if it is <= lim, else -1 */
static int bdt_rem(uint64_t m, int lim)
{
    uint64_t c[2];
    int r[2], i, minEven = -1, maxOdd = -1, unknown = 0;
    if (m == 1) return 0;
    if (lim <= 0) return -1;
    c[0] = odd_part_u64(3 * m + 1);
    c[1] = odd_part_u64(3 * m - 1);
    for (i = 0; i < 2; i++) {
        r[i] = bdt_rem(c[i], lim - 1);
        if (r[i] < 0) unknown = 1;
        else if (r[i] % 2 == 0) {
            if (minEven < 0 || r[i] < minEven) minEven = r[i];
        } else if (r[i] > maxOdd) maxOdd = r[i];
    }
    if (minEven >= 0) return minEven + 1;
    if (unknown) return -1;
    return maxOdd + 1;
}

static uint64_t *bdt_terms = NULL;
static idx_t bdt_count = 0, bdt_cap = 0;
static uint64_t bdt_next = 1;

seq_t a(idx_t n)
{
    while (bdt_count < n) {
        uint64_t m = bdt_next++;
        if (bdt_rem(m, 5) == 5) {
            if (bdt_count == bdt_cap) {
                bdt_cap = bdt_cap ? 2 * bdt_cap : 1024;
                bdt_terms = (uint64_t *)realloc(bdt_terms, bdt_cap * sizeof(uint64_t));
            }
            bdt_terms[bdt_count++] = m;
        }
    }
    return (seq_t)bdt_terms[n - 1];
}
