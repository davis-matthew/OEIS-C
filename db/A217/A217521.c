/*
 * A217521  Base-4 state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (123...n)*.
 * https://oeis.org/A217521
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{r} ( n if ∃k with 4^k ≡ r (mod n) and 4^k ≥ n else
 *            max_{k<k0} 4^k with 4^k ≡ r ), where k0 = ⌈log₄ n⌉
 * Range:     u8 n <= 28, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A217521 */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n ≥ 2 (offset starts at 2) */
    if (n < 2) return 0;

    size_t sz = (size_t)n;

    seq_t *maxP = (seq_t*)calloc(sz, sizeof(seq_t));    /* largest 4^k < n for each residue */
    bool *seen  = (bool*)calloc(sz, sizeof(bool));     /* residue occurs for some large k   */
    bool *vis   = (bool*)calloc(sz, sizeof(bool));     /* visited during later cycle       */

    if (!maxP || !seen || !vis) {
        free(maxP); free(seen); free(vis);
        return 0;
    }

    /* Small k: 4^k < n */
    idx_t pow = 1;          /* 4^k in idx_t; safe since pow < n ≤ 10⁴ → pow fits in 64‑bit */
    idx_t r   = 1 % n;      /* current residue 4^k mod n */

    while (pow < n) {
        if ((seq_t)pow > maxP[r])   /* store largest 4^k < n for this residue */
            maxP[r] = (seq_t)pow;

        pow = pow * 4;
        r   = (r * 4) % n;
    }
    /* pow ≥ n now; r is residue of the first big power */

    /* Large k: continue cycle until repeat */
    while (1) {
        if (!vis[r]) {
            vis[r] = true;
            seen[r] = true;
            r = (r * 4) % n;
        } else {
            break;
        }
    }

    /* Sum contributions */
    seq_t result = 0;
    for (size_t i = 0; i < sz; ++i) {
        if (seen[i]) {
            result += (seq_t)n;
        } else if (maxP[i] != 0) {
            result += maxP[i];
        }
    }

    free(maxP); free(seen); free(vis);
    return result;
}
