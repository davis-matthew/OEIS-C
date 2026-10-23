/*
 * A044497  Numbers n such that string 2,2 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer n such that base‑6 representation of n *
 *            contains the string "22" but base‑6 representation of n+1 does
 *            not
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has22_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has22_base6.h"

static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
static idx_t next_candidate = 1;

static void ensure_terms(idx_t n)
{
    while (terms_len < n) {
        seq_t cand = (seq_t)next_candidate;
        if (has22_base6(cand) && !has22_base6(cand + (seq_t)1)) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) {
                    exit(1);
                }
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = cand;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)0;
    }
    ensure_terms(n);
    return terms[n - 1];
}
