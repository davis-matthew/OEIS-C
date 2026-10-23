/*
 * A049296  First differences of A008364. Also first differences of reduced
 *          residue system (RRS) for 4th primorial number, A002110(4)=210.
 * https://oeis.org/A049296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence of first differences of the reduced residue system
   modulo the 4th primorial (210).  The residues coprime to 210 are the
   48 numbers 1 ≤ r < 210 with gcd(r,210)=1.  The differences between
   successive residues (wrapping around) repeat every 48 terms.  This
   function pre‑computes and returns the appropriate difference. */
#include <stddef.h>
#include <stdint.h>

/* Difference array for the 48 residues coprime to 210. */
static seq_t diffs[48];
static int diffs_initialized = 0;

/* Build the difference table once. */
static void init_diffs(void)
{
    if (diffs_initialized) return;

    /* Collect the 48 residues coprime to 210.  210 = 2·3·5·7. */
    uint64_t residues[48];
    size_t count = 0;
    for (uint64_t i = 1; i < 210ULL; ++i) {
        if (i % 2ULL == 0ULL || i % 3ULL == 0ULL ||
            i % 5ULL == 0ULL || i % 7ULL == 0ULL)
            continue;
        residues[count++] = i;
    }
    /* Compute the consecutive differences, wrapping from last to first. */
    for (size_t i = 0; i < 48; ++i) {
        size_t next = (i + 1) % 48;
        uint64_t diff;
        if (i < 47)
            diff = residues[next] - residues[i];
        else
            diff = (210ULL - residues[i]) + residues[0];
        diffs[i] = (seq_t)diff;
    }
    diffs_initialized = 1;
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    init_diffs();
    size_t idx = (size_t)((n - 1ULL) % 48ULL);
    return diffs[idx];
}
