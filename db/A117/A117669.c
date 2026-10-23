/*
 * A117669  The n-th n-gonal number divisible by n.
 * https://oeis.org/A117669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the n‑th n‑gonal number divisible by n.
   We use the congruence
        (n-2)k² - (n-4)k ≡ 0   (mod 2n)
   to obtain all residue classes k (mod 2n) for which the
   n‑gonal number P(n,k)=((n-2)k²-(n-4)k)/2 is a multiple of n.
   Then we determine the n‑th solution k by binary search over the
   cumulative count of solutions. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Count how many solutions k (1 ≤ k ≤ limit) satisfy the congruence
   k ≡ roots[i] (mod m) for the given list of residues. */
static idx_t
count_solutions(idx_t limit, const idx_t *roots, size_t cnt, idx_t m, idx_t N)
{
    idx_t total = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (limit < roots[i]) continue;
        total += (limit - roots[i]) / m + 1;
        if (total >= N) return total;
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence starts at n = 1 */

    const idx_t m = 2 * n;         /* modulus for the congruence */

    /* Find all residues r (0 ≤ r < m) that satisfy
       (n-2)r² - (n-4)r ≡ 0 (mod m). */
    size_t capacity = 8;
    size_t cnt = 0;
    idx_t *roots = (idx_t*)malloc(capacity * sizeof(idx_t));
    if (!roots) return 0;         /* allocation failure – return 0 */

    int64_t a_coef = (int64_t)n - 2;     /* n-2 */
    int64_t b_coef = (int64_t)n - 4;     /* n-4 */

    for (idx_t k = 0; k < m; ++k) {
        int64_t val = a_coef * (int64_t)k * k - b_coef * (int64_t)k;
        int64_t mod_val = val % (int64_t)m;
        if (mod_val < 0) mod_val += m;
        if (mod_val == 0) {
            if (cnt == capacity) {
                capacity <<= 1;
                idx_t *tmp = (idx_t*)realloc(roots, capacity * sizeof(idx_t));
                if (!tmp) { free(roots); return 0; }
                roots = tmp;
            }
            roots[cnt++] = k;
        }
    }

    if (cnt == 0) { free(roots); return 0; }   /* should not happen */

    /* Replace a residue of 0 by m, because k = 0 is not a valid
       polygonal number index – the first positive solution is k = m. */
    for (size_t i = 0; i < cnt; ++i)
        if (roots[i] == 0) roots[i] = m;

    /* Binary search for the n‑th solution k.  The bound k ≤ n·m is safe. */
    idx_t low = 1;
    idx_t high = n * m;       /* high is inclusive in the search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (count_solutions(mid, roots, cnt, m, n) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;            /* the desired index */

    free(roots);              /* no longer needed */

    /* Compute the desired n‑gonal number: P(n,k) = ((n-2)k²-(n-4)k)/2. */
    seq_t poly = (seq_t)a_coef * k * k - (seq_t)b_coef * k;
    poly /= 2;
    return poly;
}
