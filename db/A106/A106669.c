/*
 * A106669  Marks on lexicographically third 6-mark optimal Golomb ruler.
 * https://oeis.org/A106669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth mark of the lexicographically third optimal
 *            6‑mark Golomb ruler. The known ruler has marks 0, 1, 8, 12, 14,
 *            17. For n ≥ 6 the sequence is undefined, so this implementation
 *            returns 0 for those indices
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Marks of the 6‑mark Golomb ruler (lexicographically third optimal). */
    static const seq_t marks[6] = { 0, 1, 8, 12, 14, 17 };

    /* n is unsigned, so compare with an unsigned literal. */
    if (n < 6ULL)
        return marks[(size_t)n];
    /* Undefined for n ≥ 6. */
    return (seq_t)0;
}
