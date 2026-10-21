/*
 * A088976  Breadth-first traversal of the Collatz tree, with the even child
 *          of each node traversed prior to its odd child. If the Collatz 3n+1
 *          conjecture is true, this is a permutation of all positive
 *          integers.
 * https://oeis.org/A088976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 102, u32 n <= 3517, u64 n <= 3517,
 *            u128 n <= 3517, big any n
 * Helpers:   seen_contains, seq_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Breadth‑first traversal of the Collatz tree.
   a(0) = 1.
   For every node n in the tree, its children are
   even child: 2n,
   odd child (only when n ≡ 4 (mod 6)): (n‑1)/3.
   The sequence lists the nodes in BFS order, visiting the even child
   before the odd child.
   The implementation uses a simple linear search for duplicates
   – sufficient for the required range (up to ~10 000 terms). */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seen_contains.h"
#include "../utils/seq_grow.h"

seq_t a(idx_t n)
{
    /* static state ----------------------------------------------------- */
    static seq_t *seq = NULL;              /* all discovered nodes */
    static size_t seq_len = 0;             /* number of elements in seq */
    static size_t seq_cap = 0;             /* array capacity           */
    static size_t proc_idx = 0;            /* index of next node to expand */

    /* initialise on first call --------------------------------------- */
    if (seq_len == 0) {
        seq_cap = 64;
        seq = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        if (!seq)
            abort();
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* generate until we have at least n+1 terms ------------------------ */
    const seq_t two   = (seq_t)2;
    const seq_t three = (seq_t)3;
    const seq_t six   = (seq_t)6;

    while (seq_len <= (size_t)n) {
        if (proc_idx == seq_len)
            break;   /* should never happen */

        seq_t cur = seq[proc_idx++];
        /* even child */
        seq_t child = cur * two;
        if (!seen_contains(seq, seq_len, child)) {
            seq_grow(&seq, &seq_len, &seq_cap);
            seq[seq_len++] = child;
        }
        /* odd child (only when cur ≡ 4 (mod 6)) */
        if ((cur % six) == (seq_t)4) {
            seq_t child2 = (cur - (seq_t)1) / three;
            if (!seen_contains(seq, seq_len, child2)) {
                seq_grow(&seq, &seq_len, &seq_cap);
                seq[seq_len++] = child2;
            }
        }
    }

    return seq[(size_t)n];
}
