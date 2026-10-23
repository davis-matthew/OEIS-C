/*
 * A044455  Numbers n such that string 3,1 occurs in the base 4 representation
 *          of n but not of n+1.
 * https://oeis.org/A044455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) found by scanning integers for base‑4 pattern "31" in n but
 *            not in n+1
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   contains31_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/contains31_base4.h"

#define MAX_TERMS 200000

static seq_t terms[MAX_TERMS];
static size_t terms_count = 0;
static seq_t next_candidate = 1;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but guard against zero */

    while (terms_count < n) {
        seq_t cand = next_candidate;
        seq_t succ = cand + 1;
        if (contains31_base4(cand) && !contains31_base4(succ)) {
            if (terms_count < MAX_TERMS)
                terms[terms_count++] = cand;
        }
        next_candidate = next_candidate + 1;
    }
    return terms[n - 1];
}
