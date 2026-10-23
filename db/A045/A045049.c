/*
 * A045049  Numbers whose base-4 representation contains exactly two 0's and
 *          two 2's.
 * https://oeis.org/A045049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6519, u32 n <= 9684, u64 n <= 9684,
 *            u128 n <= 9684, big any n
 * Helpers:   choose2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth smallest positive integer whose base-4 representation
   contains exactly two 0's and two 2's.  The sequence starts at
   a(1) = 130, a(2) = 136, … (OEIS A045049).  The computation is
   performed by iterating over base‑4 digit positions in lexicographic
   order, using combinatorial counts to skip over blocks of
   candidates.  This yields an O(L) algorithm where L is the number
   of base‑4 digits of the answer; for n up to 10000 the length is ≤ 9,
   so the routine runs well under 2 s.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/choose2.h"

static inline idx_t choose(idx_t n, idx_t k) {
    if (k == 0) return 1;
    if (k == 1) return n;
    if (k == 2) return choose2(n);
    return 0; /* k > 2 is not needed here */
}

/* number of completions for a remaining segment of length rem,
   with zeros_left zeros and twos_left twos to place among it.
   The remaining digits (if any) can be 1 or 3. */
static inline idx_t count_completions(idx_t rem, idx_t zeros_left, idx_t twos_left) {
    if (zeros_left > rem || twos_left > rem - zeros_left) return 0;
    if (zeros_left + twos_left > rem) return 0;
    idx_t ones_or_threes = rem - zeros_left - twos_left;
    return choose(rem, zeros_left) *
           choose(rem - zeros_left, twos_left) *
           (1ULL << ones_or_threes);
}

seq_t a(idx_t n) {
    /* Find length L of the base‑4 representation of the answer. */
    idx_t cum = 0;
    idx_t L = 4;
    idx_t countL;
    while (true) {
        /* count for length L: C(L-1,2)*C(L-2,2)*2^(L-4) */
        countL = choose(L - 1, 2) * choose(L - 2, 2) * (1ULL << (L - 4));
        if (cum + countL >= n) break;
        cum += countL;
        ++L;
    }

    /* offset within numbers of length L (0‑based) */
    idx_t offset = n - cum - 1;

    idx_t zeros_rem = 2;
    idx_t twos_rem  = 2;
    seq_t result = 0;

    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t rem = L - pos - 1; /* positions left after current one */
        for (int d = (pos == 0 ? 1 : 0); d <= 3; ++d) {
            idx_t nz = zeros_rem;
            idx_t nt = twos_rem;
            if (d == 0) {
                if (nz == 0) continue;
                nz -= 1;
            } else if (d == 2) {
                if (nt == 0) continue;
                nt -= 1;
            }
            idx_t completions = count_completions(rem, nz, nt);
            if (offset < completions) {
                result = (result * 4) + (seq_t)d;
                zeros_rem = nz;
                twos_rem  = nt;
                break;
            } else {
                offset -= completions;
            }
        }
    }

    return result;
}
