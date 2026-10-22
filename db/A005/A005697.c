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
 * Helpers:   bdt_odd, bdt_rem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/bdt_odd.h"
#include "../utils/bdt_rem.h"

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
