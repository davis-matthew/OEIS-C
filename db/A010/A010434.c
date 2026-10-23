/*
 * A010434  Squares mod 73.
 * https://oeis.org/A010434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth smallest quadratic residue modulo 73 (including 0). */
#include <stdbool.h>
#include <stdlib.h>

/* Comparator for qsort on generic unsigned seq_t values. */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Static cache of the residues. */
    static seq_t residues[73];
    static size_t count = 0;
    static bool initialized = false;

    if (!initialized) {
        bool seen[73] = { false };
        idx_t idx = 0;
        for (idx_t x = 0; x < 73; ++x) {
            seq_t r = (seq_t)x * (seq_t)x % 73;
            idx_t r_uint = (idx_t)r;        /* r < 73 */
            if (!seen[r_uint]) {
                seen[r_uint] = true;
                residues[idx++] = r;
            }
        }
        count = idx;                       /* should be 37 for prime 73 */
        qsort(residues, count, sizeof(seq_t), cmp_seq_t);
        initialized = true;
    }

    if (n == 0 || n > count)
        return (seq_t)0;                   /* out of range */
    return residues[n - 1];
}
