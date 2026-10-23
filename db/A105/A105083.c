/*
 * A105083  Trajectory of 1 under the morphism 1 -> 12, 2 -> 3, 3 -> 1.
 * https://oeis.org/A105083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1176, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function computes a(n) for OEIS sequence A105083 (trajectory of 1
   under the morphism 1 → 12, 2 → 3, 3 → 1).  The value at index n
   is the nth symbol (0‑based) of the infinite morphic word obtained
   by iterating the morphism from the initial symbol 1.  The algorithm
   uses a static table of expansion lengths and a recursive descent
   that selects the appropriate symbol without generating the full word. */

#include <stdint.h>

static seq_t len_table[128][3];   /* len_table[depth][s] gives length of s after depth steps */
static int max_depth = 0;         /* highest precomputed depth */

/* Initialise the table for depth 0 */
static void init_length_table(void)
{
    if (max_depth == 0) {
        len_table[0][0] = 1;  /* length of '1' after 0 expansions */
        len_table[0][1] = 1;  /* length of '2' after 0 expansions */
        len_table[0][2] = 1;  /* length of '3' after 0 expansions */
    }
}

/* Extend the table until len_table[depth][0] > n. */
static void ensure_depth(idx_t n)
{
    init_length_table();
    while ((idx_t)len_table[max_depth][0] <= n) {
        /* If we run out of space, stop extending. */
        if (max_depth + 1 >= (int)(sizeof(len_table) / sizeof(len_table[0]))) {
            break;
        }
        int d = max_depth;
        /* f(1) = 12 → indices 0,1 */
        len_table[d + 1][0] = len_table[d][0] + len_table[d][1];
        /* f(2) = 3  → index 2 */
        len_table[d + 1][1] = len_table[d][2];
        /* f(3) = 1  → index 0 */
        len_table[d + 1][2] = len_table[d][0];
        max_depth++;
    }
}

/* Recursive helper: returns the symbol (1,2,or 3) at position n
   within the expansion of symbol s (0✕→'1', 1✕→'2', 2✕→'3') after
   depth steps.  depth is chosen so that len_table[depth][s] > n. */
static seq_t get_symbol(int s, idx_t n, int depth)
{
    if (depth == 0) {
        return (seq_t)(s + 1);   /* base case: length is 1 */
    }

    switch (s) {
        case 0: {   /* 1 → 12 (indices 0,1) */
            seq_t l0 = len_table[depth - 1][0];
            if ((seq_t)n < l0) {
                return get_symbol(0, n, depth - 1);
            }
            n -= (idx_t)l0;
            seq_t l1 = len_table[depth - 1][1];
            if ((seq_t)n < l1) {
                return get_symbol(1, n, depth - 1);
            }
            n -= (idx_t)l1;   /* should not happen */
            return 0;
        }
        case 1: {   /* 2 → 3 (index 2) */
            seq_t l = len_table[depth - 1][2];
            if ((seq_t)n < l) {
                return get_symbol(2, n, depth - 1);
            }
            return 0;
        }
        case 2: {   /* 3 → 1 (index 0) */
            seq_t l = len_table[depth - 1][0];
            if ((seq_t)n < l) {
                return get_symbol(0, n, depth - 1);
            }
            return 0;
        }
    }
    return 0;  /* unreachable */
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    ensure_depth(n);
    return get_symbol(0, n, max_depth);
}
