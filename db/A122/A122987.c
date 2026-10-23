/*
 * A122987  Possible last three digits of n^3 (leading zeros omitted).
 * https://oeis.org/A122987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, easy, nonn
 * Formula:   a(n) = the nth smallest residue r (0 ≤ r < 1000) for which there
 *            exists an integer m ≥ 1 with m³ ≡ r (mod 1000). The sequence
 *            terminates at n = 505 (a(505) = 999). It is equivalent to the
 *            set of cubic residues modulo 1000, sorted in ascending order
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 505, u64 n <= 505, u128 n <= 505,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t c122987_cache[505];
static bool c122987_inited = false;

/* Helper: compute all cubic residues modulo 1000 and store them sorted
   in ascending order in c122987_cache.  The array is filled only once. */
static void c122987_init(void)
{
    bool occ[1000] = {false};
    seq_t r;
    /* Compute residues for m = 1..1000 (full period modulo 1000). */
    for (idx_t m = 1; m <= 1000; ++m) {
        seq_t v = (seq_t)m * m * m;
        r = v % (seq_t)1000;
        occ[(int)r] = true;
    }
    /* Collect sorted residues into the cache. */
    idx_t k = 0;
    for (idx_t i = 0; i < 1000; ++i) {
        if (occ[(int)i]) {
            c122987_cache[k++] = (seq_t)i;
        }
    }
    /* Sanity: k should be 505. */
    (void)k; /* Suppress unused variable warning if sequence size changes. */
    c122987_inited = true;
}

seq_t a(idx_t n)
{
    if (!c122987_inited) {
        c122987_init();
    }
    if (n == 0) {
        /* Undefined for n < 1; return 0 as safe fallback. */
        return (seq_t)0;
    }
    if (n > 505) {
        /* Sequence is undefined beyond this point; return 0. */
        return (seq_t)0;
    }
    return c122987_cache[n - 1];
}
