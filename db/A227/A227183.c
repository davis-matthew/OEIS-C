/*
 * A227183  a(n) is the sum of parts of the unique unordered partition encoded
 *          in the run lengths of the binary expansion of n; row sums of
 *          A227739 for n >= 1.
 * https://oeis.org/A227183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = sum of the partial sums of the run‑lengths of the binary
 *            * expansion of n after subtracting one from all but the first
 *            run. * Equivalently: * a(0) = 0 * For n > 0: * Let the sequence
 *            of runs (starting from the least significant bit) be * L[0],
 *            L[1], … , L[m‑1]. * Set T[0] = L[0], T[i] = L[...
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 0 */
        return (seq_t)0;

    seq_t sum = 0;       /* final result */
    seq_t cum = 0;       /* running cumulative sum of T[i] */
    idx_t  runIndex = 0; /* index of current run (0-based) */

    while (n > 0)
    {
        /* Count a run of identical bits starting from the LSB. */
        idx_t  bit = n & 1ULL;   /* current bit value 0 or 1 */
        idx_t  len = 0;          /* run length */

        while (n > 0 && (n & 1ULL) == bit)
        {
            len++;
            n >>= 1;
        }

        /* Apply the “subtract one from runs after the first”. */
        seq_t t = (seq_t)len;          /* T[runIndex] */
        if (runIndex > 0)
            t -= (seq_t)1;             /* t = len - 1 for runIndex > 0 */

        cum += t;                      /* update cumulative sum */
        sum += cum;                    /* add to final sum */

        runIndex++;                    /* move to next run */
    }

    return sum;
}
