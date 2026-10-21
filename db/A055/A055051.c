/*
 * A055051  Numbers of the form 2^(2i+1)*(8j+7), 2^(2i+1)*(16j+6) or
 *          2^(2i+1)*(4j+3).
 * https://oeis.org/A055051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth term of the multiset of numbers that can be *
 *            written in any of the following three ways: * 2^(2i+1)*(8j+7) ,
 *            2^(2i+1)*(16j+6) or 2^(2i+1)*(4j+3) * (i,j ≥ 0). The sequence
 *            contains each representation as a separate * element and is
 *            sorted in non‑decreasing order. The first few ter...
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   multiplicity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/multiplicity.h"

/* static storage for the generated sequence */
static seq_t  *table      = NULL;
static size_t table_len   = 0;
static size_t table_cap   = 0;
static seq_t  next_candidate = 1;

seq_t a(idx_t n)
{
    /* generate enough terms to satisfy the requested index */
    while (table_len < n) {
        seq_t x = next_candidate;
        unsigned int mult = multiplicity(x);

        for (unsigned int k = 0; k < mult; ++k) {
            if (table_len == table_cap) {
                table_cap = (table_cap == 0) ? 16 : table_cap * 2;
                table = (seq_t *)realloc(table, table_cap * sizeof(seq_t));
            }
            table[table_len++] = x;
        }
        ++next_candidate;
    }
    /* a(1) is the first stored term */
    return table[n - 1];
}
