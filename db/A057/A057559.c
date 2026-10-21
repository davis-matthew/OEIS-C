/*
 * A057559  Lexicographic ordering of NxNxNxN, where N={1,2,3,...}.
 * https://oeis.org/A057559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 140, u16 n <= 140, u32 n <= 140, u64 n <= 140,
 *            u128 n <= 140, big any n
 * Helpers:   nC2, nC3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A057559: lexicographic ordering of quadruples of positive
   integers sorted first by sum, then lexicographically.
   For n>=1, a(n) is the coordinate at position (n-1)%4 in the
   ((n-1)/4)-th tuple (0‑based). */
#include <stddef.h>
#include "../utils/nC2.h"
#include "../utils/nC3.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* zero‑based tuple index and component position */
    idx_t tuple_index = (n - 1) / 4;
    int   pos         = (int)((n - 1) % 4);

    /* Find the sum s that contains this tuple */
    idx_t s = 4;          /* minimal sum for positive 4‑tuples */
    idx_t cumulative = 0; /* number of tuples with sums < s */
    while (1) {
        idx_t tuples_of_s = nC3(s - 1);          /* C(s-1,3) */
        if (tuple_index < cumulative + tuples_of_s)
            break;
        cumulative += tuples_of_s;
        ++s;
    }

    /* Offset within the block of tuples with sum s */
    idx_t offset = tuple_index - cumulative;

    /* Recover the lexicographic tuple (a,b,c,d) with sum s */
    idx_t a_val = 0, b_val = 0, c_val = 0, d_val = 0;
    idx_t rem = s; /* remaining sum to distribute */

    /* Choose a */
    for (idx_t poss_a = 1; poss_a <= rem - 3; ++poss_a) {
        idx_t count_a = nC2(rem - poss_a - 1);
        if (offset >= count_a) {
            offset -= count_a;
            continue;
        }
        a_val = poss_a;
        rem -= poss_a;
        break;
    }

    /* Choose b */
    for (idx_t poss_b = 1; poss_b <= rem - 2; ++poss_b) {
        idx_t count_b = rem - poss_b - 1; /* (rem-b-1) ways for c,d */
        if (offset >= count_b) {
            offset -= count_b;
            continue;
        }
        b_val = poss_b;
        rem -= poss_b;
        break;
    }

    /* Remaining two coordinates */
    c_val = offset + 1;        /* 1-indexed */
    d_val = rem - c_val;       /* the rest of the sum */

    /* Return the requested component */
    switch (pos) {
        case 0: return (seq_t)a_val;
        case 1: return (seq_t)b_val;
        case 2: return (seq_t)c_val;
        case 3: return (seq_t)d_val;
        default: return (seq_t)0; /* never reached */
    }
}
