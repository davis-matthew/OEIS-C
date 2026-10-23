/*
 * A161978  a(n) = the smallest positive integer that when written in binary
 *          contains both binary n and {the string formed from the reversal of
 *          the binary digits of n, with leading 0's removed} as (possibly
 *          overlapping) substrings.
 * https://oeis.org/A161978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, look, nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 642, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   bin_to_seq, overlap_len, rev_no_leading, str_contains,
 *            to_bin_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A161978 – smallest positive integer whose binary form
   contains both the binary representation of n and the binary
   representation of the reversal of n with leading zeros removed.
   The algorithm builds the two patterns and returns the shorter
   string that contains both as substrings. */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/bin_to_seq.h"
#include "../utils/overlap_len.h"
#include "../utils/rev_no_leading.h"
#include "../utils/str_contains.h"
#include "../utils/to_bin_string.h"

seq_t a(idx_t n)
{
    /* Patterns for n and its reversed binary (without leading zeros). */
    char p[128], q[128];
    size_t lp = to_bin_string(n, p);
    size_t lq = rev_no_leading(n, q);

    /* Candidate 1: p followed by q, with maximal overlap. */
    char cand1[256];
    size_t cand1_len;
    if (str_contains(p, q)) {            /* q is already inside p */
        memcpy(cand1, p, lp);
        cand1_len = lp;
    } else {
        size_t ol = overlap_len(p, lp, q, lq);
        cand1_len = lp + lq - ol;
        memcpy(cand1, p, lp);
        memcpy(cand1 + lp, q + ol, lq - ol);
    }
    cand1[cand1_len] = '\0';

    /* Candidate 2: q followed by p, with maximal overlap. */
    char cand2[256];
    size_t cand2_len;
    if (str_contains(q, p)) {
        memcpy(cand2, q, lq);
        cand2_len = lq;
    } else {
        size_t ol = overlap_len(q, lq, p, lp);
        cand2_len = lq + lp - ol;
        memcpy(cand2, q, lq);
        memcpy(cand2 + lq, p + ol, lp - ol);
    }
    cand2[cand2_len] = '\0';

    /* Choose the shorter string, breaking ties lexicographically. */
    if (cand1_len < cand2_len || (cand1_len == cand2_len && strcmp(cand1, cand2) <= 0))
        return bin_to_seq(cand1);
    else
        return bin_to_seq(cand2);
}
