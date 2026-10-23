/*
 * A014465  A063691 without zeros.
 * https://oeis.org/A014465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8283, u16 n <= 8283, u32 n <= 8283, u64 n <= 8283,
 *            u128 n <= 8283, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A063691:  a(n) is the number of ordered triples
 * (x,y,z) of positive integers such that x²+y²+z² equals the nth
 * distinct sum of three positive squares.  No closed formula is
 * known, so we enumerate all sums ≤ MAX_SUM, count their
 * multiplicities, and store the counts in ascending order.
 * The array is built lazily on the first call. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Bound for enumeration – large enough for n≈10 000.
 * For n up to about 10 000 the first 20 000 distinct sums are < 200 000. */
#define MAX_SUM 200000

/* Frequency of each sum 1..MAX_SUM.  Zero-initialised statically. */
static uint32_t freq[MAX_SUM + 1];

/* Array holding the multiplicities of the distinct sums, in
 * ascending order.  Zero-initialised statically; entries beyond
 * a_len are unused. */
static seq_t a_vals[MAX_SUM + 1];

/* Number of distinct sums found and the flag that the tables have
 * been built.  These are written only once. */
static idx_t a_len = 0;
static int built = 0;

/* Build the tables on first use.  The function is re‑entrant and does
 * nothing on subsequent calls. */
static void build_tables(void)
{
    if (built)
        return;

    /* Enumerate all triples (x,y,z) with x,y,z ≥ 1 and count sums. */
    for (idx_t x = 1; x * x <= MAX_SUM; ++x) {
        idx_t xsq = x * x;
        for (idx_t y = 1; xsq + y * y <= MAX_SUM; ++y) {
            idx_t xy = xsq + y * y;
            for (idx_t z = 1; xy + z * z <= MAX_SUM; ++z) {
                idx_t s = xy + z * z;
                freq[s]++;          /* each ordered triple counted once */
            }
        }
    }

    /* Extract distinct sums in ascending order and store their counts. */
    a_len = 0;
    for (size_t i = 1; i <= MAX_SUM; ++i) {
        if (freq[i]) {
            a_vals[a_len++] = (seq_t)freq[i];
        }
    }
    built = 1;
}

/* Public interface.  For n < 1 or n > a_len return 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    build_tables();
    if (!built || n > a_len)
        return (seq_t)0;
    return a_vals[n - 1];
}
