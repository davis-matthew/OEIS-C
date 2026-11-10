/*
 * A094426  Terms of A094302 without repetition.
 * https://oeis.org/A094426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th distinct value in order of first appearance of
 *            A094302, where A094302: b(1) = 1; b(m) = m*b(m-1) if m > b(m-1),
 *            else b(m) = b(m-1)/gcd(b(m-1), m)
 * Range:     u8 n <= 14, u16 n <= 36, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t A094426_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t r = x % y;
        x = y;
        y = r;
    }
    return x;
}

static seq_t *A094426_terms = 0;
static size_t A094426_cnt = 0;
static size_t A094426_cap = 0;
static seq_t A094426_cur = 1;
static idx_t A094426_i = 1;

static void A094426_push(seq_t v)
{
    if (A094426_terms == 0) {
        A094426_cap = 256;
        A094426_terms = mem_new(seq_t, A094426_cap);
    } else if (A094426_cnt == A094426_cap) {
        A094426_cap *= 2;
        A094426_terms = mem_resize(seq_t, A094426_terms, A094426_cap);
    }
    A094426_terms[A094426_cnt++] = v;
}

static bool A094426_seen(seq_t v)
{
    for (size_t k = A094426_cnt; k > 0; --k) {
        if (A094426_terms[k - 1] == v)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (A094426_cnt == 0)
        A094426_push((seq_t)1);
    while ((idx_t)A094426_cnt < n) {
        A094426_i++;
        seq_t ii = (seq_t)A094426_i;
        if (ii > A094426_cur) {
            A094426_cur = A094426_cur * ii;
        } else {
            seq_t g = A094426_gcd(A094426_cur, ii);
            A094426_cur = A094426_cur / g;
        }
        if (!A094426_seen(A094426_cur))
            A094426_push(A094426_cur);
    }
    return A094426_terms[(size_t)(n - 1)];
}
