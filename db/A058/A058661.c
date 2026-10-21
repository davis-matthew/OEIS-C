/*
 * A058661  McKay-Thompson series of class 39C for Monster.
 * https://oeis.org/A058661
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 58, u32 n <= 185, u64 n <= 628,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(-1)=1; for n ≥ 0, a(n) = p_{3,13}(n+1) – δ_{n,0},
 * where p_{3,13}(m) counts partitions of m into parts not divisible by 3 or 13.
 *
 * The series expansion of –1 + (eta(q³)·eta(q¹³))/(eta(q)·eta(q³⁹))
 * begins 1/q + 2q + 2q² + 4q³ + …, giving a(0)=0 and the listed terms.
 */

#include <stddef.h>                 /* size_t */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Static cache of partition numbers p_{3,13}(k) for k = 0..h_nmax */
static seq_t *H = NULL;
static size_t h_nmax = 0;

/* Recompute the entire array up to newmax (inclusive). */
static void recompute_h(size_t newmax)
{
    seq_t *newH = seq_alloc_new_array(newmax + 1);
    /* initialise with zeros */
    for (size_t i = 0; i <= newmax; ++i) newH[i] = 0;
    newH[0] = 1;                           /* base case */

    /* Dynamic programming: add each allowed part size s. */
    for (size_t s = 1; s <= newmax; ++s) {
        if (s % 3 == 0 || s % 13 == 0) continue;   /* forbidden part */
        for (size_t i = s; i <= newmax; ++i) {
            newH[i] = newH[i] + newH[i - s];
        }
    }

    if (H != NULL) seq_free_array(H);
    H = newH;
    h_nmax = newmax;
}

/* Ensure that H[0..need] are computed. */
static void ensure_h(size_t need)
{
    if (need <= h_nmax) return;
    recompute_h(need);
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Off‑by‑one offset: n = -1 gives the first coefficient (1/q). */
    if (n < -1) return 0;        /* undefined for indices below the offset */

    if (n == -1) return 1;

    /* n ≥ 0 → need to have p_{3,13}(n+1). */
    size_t need = (size_t)(n + 1);
    ensure_h(need);

    if (n == 0) {
        /* a(0) = p_{3,13}(1) – 1  == 1 – 1 = 0  */
        seq_t val = H[1];
        val = val - (seq_t)1;
        return val;
    }
    return H[need];
}
