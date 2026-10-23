/*
 * A104078  Numbers which are the sum of three nonzero squares and are also
 *          divisible by 31.
 * https://oeis.org/A104078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1759, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A104078
 * a(n) = the n-th integer divisible by 31 that can be expressed
 *        as a sum of three non‑zero squares.
 * This implementation builds all sums of two positive squares
 * up to each new candidate and tests each multiple of 31 until
 * the requested term is found.  Results are memoised so that
 * arbitrary (unordered) calls to a(idx_t n) are answered in
 * O(n) time.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* Table of whether a number <= sum_table_limit is a sum of two
 * positive squares.  Value 1 means yes, 0 means no.  The table
 * grows monotonically as new candidates appear. */
static unsigned char *sum_two_table = NULL;
static idx_t sum_table_limit = 0;

/* Memoised sequence values (1‑based).  terms[i-1] = a(i). */
static seq_t *terms = NULL;
static idx_t terms_alloc = 0;
static idx_t terms_count = 0;

/* Next candidate multiple of 31 to examine.  The first such value
 * (62) is known to work. */
static seq_t next_candidate = (seq_t)62;

/* Make sure sum_two_table covers all x ≤ limit.  The table is
 * increased by reallocating to the new size and zeroing the new
 * entries.  The full table is rebuilt on each grow step, which
 * is inexpensive for the required sizes. */
static void ensure_sum_table(idx_t limit)
{
    if (limit <= sum_table_limit)
        return;

    idx_t new_limit = limit;
    unsigned char *new_table = (unsigned char*)
        realloc(sum_two_table, (size_t)(new_limit + 1));
    if (!new_table) return;          /* allocation failure unlikely */

    if (new_limit + 1 > sum_table_limit + 1)
        memset(new_table + sum_table_limit + 1, 0,
               (size_t)(new_limit + 1 - (sum_table_limit + 1)));

    /* Fill in the new entries.  All entries up to new_limit are
     * already zeroed. */
    for (idx_t a = 1; a * a <= new_limit; ++a) {
        idx_t a2 = a * a;
        for (idx_t b = 1; a2 + b * b <= new_limit; ++b) {
            new_table[a2 + b * b] = 1;
        }
    }

    sum_two_table = new_table;
    sum_table_limit = new_limit;
}

/* Ensure terms[] can hold at least size elements.  Size grows by
 * doubling until the needed size is reached. */
static void ensure_terms_alloc(idx_t size)
{
    if (size <= terms_alloc)
        return;

    idx_t new_alloc = terms_alloc > 0 ? terms_alloc * 2 : 8;
    while (new_alloc < size) new_alloc *= 2;

    seq_t *new_terms = (seq_t*)realloc(terms, new_alloc * sizeof(seq_t));
    if (!new_terms) return; /* allocation failure unlikely */

    terms = new_terms;
    terms_alloc = new_alloc;
}

/* Main user function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n = 0 */

    /* Return the cached value if already computed. */
    if (n <= terms_count)
        return terms[n - 1];

    /* Generate terms until we have at least n of them. */
    while (terms_count < n) {
        seq_t m = next_candidate;

        /* Ensure we can look up sums of two squares up to m. */
        ensure_sum_table((idx_t)m);

        /* Test m for representation as sum of three positive squares. */
        bool ok = false;
        seq_t limit_minus_min = m - (seq_t)2;   /* a^2 ≤ m-2 */

        for (idx_t a = 1; ; ++a) {
            seq_t a2 = (seq_t)a * a;
            if (a2 > limit_minus_min) break;

            idx_t r = (idx_t)(m - a2);   /* r ≥ 2 due to loop bound */
            if (r >= 2 && sum_two_table[r]) {
                ok = true;
                break;
            }
        }

        if (ok) {
            ensure_terms_alloc(terms_count + 1);
            terms[terms_count] = m;
            terms_count++;
        }

        next_candidate += (seq_t)31;
    }

    return terms[n - 1];
}
