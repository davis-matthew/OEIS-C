/*
 * A082002  Square array read by antidiagonals, alternating upwards and
 *          downwards: T(1, 1) = 1 and every other entry is the smallest
 *          number not already used that has a common factor with every number
 *          in its row and column, not including the 1.
 * https://oeis.org/A082002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Computation of OEIS A082002 via antidiagonal filling with gcd
 *            constraints
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   ensure_used_cap, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/ensure_used_cap.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Determine how many diagonals are needed to produce `n` terms. */
    size_t diag = 1;
    while (diag * (diag + 1) / 2 < (size_t)n) ++diag;
    size_t dim = diag + 1;                       /* grid size (1‑based) */

    /* Allocate a square table large enough for all diagonal cells.   */
    size_t size = (dim + 2) * (dim + 2);          /* +2 for safety   */
    seq_t *table = (seq_t *)calloc(size, sizeof(seq_t));
    if (!table) return (seq_t)0;                  /* allocation failure */

    /* Short macro for 2‑D indexing.                           */
#define IDX(i,j) (((i) * (dim + 2)) + (j))

    table[IDX(1,1)] = (seq_t)1;                   /* first cell */

    /* Bitset of numbers that are already used.                 */
    size_t used_cap = 2;
    uint8_t *used = (uint8_t *)calloc(used_cap, sizeof(uint8_t));
    if (!used) { free(table); return (seq_t)0; }
    used[1] = 1;                                   /* 1 is considered used */

    size_t count = 1;   /* number of terms produced so far */

    /* Process diagonals one by one.                            */
    for (size_t d = 2; d <= diag; ++d) {
        if (d % 2 == 0) {                    /* even: downward direction */
            for (size_t i = 1; i <= d; ++i) {
                size_t j = d - i + 1;
                if (i == 1 && j == 1) continue;      /* already set */

                uint64_t cand = 2;
                for (;;) {
                    ensure_used_cap(&used_cap, &used, cand);
                    if (used[cand]) { ++cand; continue; }

                    bool ok = true;

                    /* check row entries to the left */
                    for (size_t k = 1; k < j; ++k) {
                        seq_t v = table[IDX(i,k)];
                        if (!v || v == (seq_t)1) continue;
                        if (gcd((seq_t)cand, v) == (seq_t)1) { ok = false; break; }
                    }
                    if (!ok) { ++cand; continue; }

                    /* check column entries above */
                    for (size_t r = 1; r < i; ++r) {
                        seq_t v = table[IDX(r,j)];
                        if (!v || v == (seq_t)1) continue;
                        if (gcd((seq_t)cand, v) == (seq_t)1) { ok = false; break; }
                    }
                    if (!ok) { ++cand; continue; }

                    break;          /* candidate accepted */
                }

                seq_t val = (seq_t)cand;
                table[IDX(i,j)] = val;
                used[cand] = 1;
                if (++count == (size_t)n) {
                    seq_t result = val;
                    free(table);
                    free(used);
                    return result;
                }
            }
        } else {                            /* odd: upward direction */
            for (size_t ii = d; ii >= 1; --ii) {
                size_t i = ii;
                size_t j = d - i + 1;
                if (i == 1 && j == 1) continue;

                uint64_t cand = 2;
                for (;;) {
                    ensure_used_cap(&used_cap, &used, cand);
                    if (used[cand]) { ++cand; continue; }

                    bool ok = true;

                    for (size_t k = 1; k < j; ++k) {
                        seq_t v = table[IDX(i,k)];
                        if (!v || v == (seq_t)1) continue;
                        if (gcd((seq_t)cand, v) == (seq_t)1) { ok = false; break; }
                    }
                    if (!ok) { ++cand; continue; }

                    for (size_t r = 1; r < i; ++r) {
                        seq_t v = table[IDX(r,j)];
                        if (!v || v == (seq_t)1) continue;
                        if (gcd((seq_t)cand, v) == (seq_t)1) { ok = false; break; }
                    }
                    if (!ok) { ++cand; continue; }

                    break;          /* candidate accepted */
                }

                seq_t val = (seq_t)cand;
                table[IDX(i,j)] = val;
                used[cand] = 1;
                if (++count == (size_t)n) {
                    seq_t result = val;
                    free(table);
                    free(used);
                    return result;
                }
                if (ii == 1) break;  /* avoid underflow when ii is size_t */
            }
        }
    }

    /* Fallback: the needed term has not been produced (should not happen). */
    seq_t result = table[IDX(1,1)];
    free(table);
    free(used);
    return result;

#undef IDX
}
