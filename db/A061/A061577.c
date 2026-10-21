/*
 * A061577  Sequence and first differences (A061578) include all numbers.
 * https://oeis.org/A061577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   greedy generation of the lexicographically earliest permutation
 *            of the positive integers that, together with all its first
 *            differences, contains every positive integer exactly once.
 *            a(1)=2, a(2)=3. For n>2: choose the smallest integer x> a(n-1)
 *            such that x and (x-a(n-1)) have neither appeared as...
 * Range:     u8 n <= 19, u16 n <= 347, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* dynamic arrays holding the sequence and the set of used numbers */
static seq_t *seq      = NULL;
static idx_t  seq_len  = 0;
static idx_t  seq_cap  = 0;

static uint8_t *used      = NULL;
static idx_t   used_cap   = 0;

/* enlarge the used array to cover index v (inclusive) */
static void ensure_used(idx_t v)
{
    if (v < used_cap) return;
    idx_t new_cap = used_cap ? used_cap * 2 : 64;
    while (new_cap <= v) new_cap <<= 1;
    uint8_t *tmp = (uint8_t*)realloc(used, new_cap);
    if (!tmp) abort();               /* out of memory */
    for (idx_t i = used_cap; i < new_cap; ++i) tmp[i] = 0;
    used = tmp;
    used_cap = new_cap;
}

/* initialise sequence and used set for the first two terms */
static void init_sequence(void)
{
    if (seq) return;

    seq_cap = 4;
    seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    if (!seq) abort();
    seq[0] = (seq_t)2ULL;
    seq[1] = (seq_t)3ULL;
    seq_len = 2;

    used_cap = 64;
    used = (uint8_t*)calloc(used_cap, 1);
    if (!used) abort();

    /* mark 1,2,3 as used (differences and terms) */
    ensure_used(3);
    used[1] = 1;
    used[2] = 1;
    used[3] = 1;
}

/* generate sequence up to index n (1‑based) */
static void generate_upto(idx_t n)
{
    init_sequence();
    while (seq_len < n) {
        idx_t last = (idx_t)seq[seq_len - 1];
        idx_t candidate = last + 1;

        /* search for the smallest candidate > last with
           both candidate and candidate-last unused */
        while (true) {
            /* ensure storage for candidate and its difference */
            ensure_used(candidate);
            idx_t diff = candidate - last;
            ensure_used(diff);

            if (!used[candidate] && !used[diff]) break;
            ++candidate;
        }

        /* append the new term */
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            if (!tmp) abort();
            seq = tmp;
        }
        seq[seq_len] = (seq_t)candidate;
        ++seq_len;

        /* mark both numbers as used */
        used[candidate] = 1;
        used[candidate - last] = 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, return 0 */
    generate_upto(n);
    return seq[n - 1];
}
