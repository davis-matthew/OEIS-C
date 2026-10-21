/*
 * A088599  Smallest multiple of n using only one nonzero digit.
 * https://oeis.org/A088599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 156, u64 n <= 6513,
 *            u128 n <= 10000, big any n
 * Helpers:   build_candidate, cmp_str, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Calculate the smallest positive multiple of n that uses exactly one
 *  non‑zero decimal digit (1–9) and any number of zeros.
 *
 *  For each digit d∈{1,…,9} we search the directed graph of remainders
 *  modulo n, where from a remainder r we can go to (r·10+0) mod n and
 *  (r·10+d) mod n.  A breadth‑first search finds the shortest string of
 *  digits {0,d} that ends in remainder 0 – i.e. the smallest multiple
 *  using only that digit.  The best result over all d is returned.
 *
 *  Complexity: O(9·n) time, O(n) memory per digit.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"
#include "../utils/build_candidate.h"
#include "../utils/cmp_str.h"

/*  Main sequence function requested by the build framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1 → n≥1 expected */

    char *best = NULL;
    size_t bestlen = 0;
    bool hasbest = false;

    for (int d = 1; d <= 9; ++d) {
        char *cand = NULL;
        size_t candlen = 0;
        build_candidate(n, d, &cand, &candlen);

        int cmp = 0;
        if (!hasbest) cmp = -1;          /* any candidate beats none */
        else cmp = cmp_str(cand, candlen, best, bestlen);

        if (cmp < 0) {                  /* candidate is smaller */
            if (hasbest) free(best);
            best = cand;
            bestlen = candlen;
            hasbest = true;
        } else {
            free(cand);
        }
    }

    seq_t value = seq_from_str(best);
    free(best);
    return value;
}
