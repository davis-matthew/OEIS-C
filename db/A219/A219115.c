/*
 * A219115  Numbers whose squares have at least one 1 and one 2 in ternary
 *          representation.
 * https://oeis.org/A219115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS a(n) = nth integer x such that x^2 in base 3 contains both digits 1 and 2
 *
 * The function keeps an internal dynamic list of already found values and
 * extends it until the requested index is available.  This guarantees
 * correct results for arbitrary call orders while keeping the runtime fast
 * (≈O(n) for the nth term).
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Return true iff x (base‑10) contains both digits 1 and 2 when written
 * in base‑3.  x is expected to be a square of a positive integer. */
static bool has1and2(seq_t x)
{
    bool has1 = false, has2 = false;
    while (x > 0) {
        uint8_t digit = (uint8_t)(x % 3);
        if (digit == 1)
            has1 = true;
        else if (digit == 2)
            has2 = true;
        if (has1 && has2)
            return true;
        x /= 3;
    }
    return has1 && has2;
}

static bool has1and2(seq_t x);

/* Return the nth term of A219115 (1‑based indexing). */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;        /* array of found terms */
    static size_t seq_len = 0;       /* number of terms stored */
    static size_t seq_cap = 0;       /* allocated capacity */
    static idx_t last_checked = 1;   /* next candidate to test */

    if (n == 0)                /* undefined, but return 0 safely */
        return 0;

    /* Extend the list until it contains at least n terms. */
    while (seq_len < (size_t)n) {
        seq_t sq = (seq_t)last_checked * (seq_t)last_checked;
        if (has1and2(sq)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, seq, new_cap);
                if (!tmp) abort();  /* out of memory */
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)last_checked;
        }
        ++last_checked;
    }

    return seq[(size_t)(n - 1)];   /* 1‑based to 0‑based index */
}
