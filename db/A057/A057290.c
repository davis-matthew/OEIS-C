/*
 * A057290  Numbers n such that n | 12^n + 11^n + 10^n + 9^n + 8^n + 7^n + 6^n
 *          + 5^n + 4^n + 3^n + 2^n.
 * https://oeis.org/A057290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer m such that m | 12^m + 11^m + …
 *            + 2^m
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

static seq_t *seq = NULL;          /* array of previously found terms */
static size_t seq_cap = 0;         /* allocated capacity of seq */
static size_t seq_len = 0;         /* number of stored terms */
static idx_t next_candidate = 2;   /* next integer to test (>=2) */

/* Store a new term in the dynamic array */
static void push_term(seq_t val)
{
    if (seq_len >= seq_cap) {
        seq_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
        seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* not used, but safe guard */

    /* initialise on first call */
    if (seq_len == 0) {
        seq_cap = 16;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq[0] = 1;          /* a(1) = 1 */
        seq_len = 1;
    }

    /* generate terms until we have at least n */
    while (seq_len < n) {
        idx_t m = next_candidate++;
        /* skip 1 (already handled), even, multiples of 3 or 5 */
        if (m == 1) continue;
        if ((m & 1) == 0) continue;
        if (m % 3 == 0) continue;
        if (m % 5 == 0) continue;

        /* The sequence elements (except 1) are all divisible by 7 or 11.
           Skip any candidate that fails this obvious filter. */
        if (m % 7 != 0 && m % 11 != 0) continue;

        seq_t mm = (seq_t)m;         /* modulus for modular arithmetic */
        seq_t sum = 0;

        /* compute S(m) modulo m = sum_{k=2}^{12} k^m (mod m) */
        for (int k = 2; k <= 12; ++k) {
            sum += mod_pow((seq_t)k, m, mm);
        }

        if (sum % mm == 0) {  /* m divides S(m) */
            push_term(mm);
        }
    }

    return seq[n - 1];
}
