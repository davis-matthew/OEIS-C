/*
 * A015590  Inverse of 1581st cyclotomic polynomial.
 * https://oeis.org/A015590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{s⊆{1,51,93,527}} (-1)^|s| * G[n-Σs] where * G[m]
 *            counts the number of unordered representations of m as a sum of
 *            * 3, 17 and 31. The sequence is periodic with period 1581
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t G[1581];
static seq_t A[1581];
static bool initialized = false;

static void init_cache(void)
{
    if (initialized)
        return;
    /* compute G: number of representations of m using parts {3,17,31}
     * (unordered).  Standard coin‑change dynamic programming. */
    for (size_t i = 0; i < 1581; ++i)
        G[i] = 0;
    G[0] = 1;
    const int parts[] = {3, 17, 31};
    for (int pi = 0; pi < 3; ++pi) {
        int d = parts[pi];
        for (size_t i = d; i < 1581; ++i)
            G[i] += G[i - d];
    }

    /* precompute subset sums and parity for the set {1,51,93,527} */
    const int neg_parts[] = {1, 51, 93, 527};
    static size_t sum_mask[16];
    static int bits_mask[16];
    for (int mask = 0; mask < 16; ++mask) {
        size_t s = 0;
        int bits = 0;
        for (int j = 0; j < 4; ++j) {
            if (mask & (1 << j)) {
                s += (size_t)neg_parts[j];
                ++bits;
            }
        }
        sum_mask[mask] = s;
        bits_mask[mask] = bits;
    }

    /* compute one period of A */
    for (size_t i = 0; i < 1581; ++i) {
        seq_t v = 0;
        for (int mask = 0; mask < 16; ++mask) {
            size_t s = sum_mask[mask];
            if (i >= s) {
                if ((bits_mask[mask] & 1) == 0)
                    v += G[i - s];
                else
                    v -= G[i - s];
            }
        }
        A[i] = v;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    init_cache();
    const size_t period = 1581;
    return A[(size_t)(n % period)];
}
