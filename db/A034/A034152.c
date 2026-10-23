/*
 * A034152  Number of partitions of n into 3 distinct parts from [ 1,9 ].
 * https://oeis.org/A034152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Number of partitions of n into 3 distinct parts from 1..9.
 * Computed by brute‑force enumeration of all 84 triples {i<j<k}.
 */
seq_t a(idx_t n)
{
    /* Any n outside [6,24] has no such partitions. */
    if (n < 6 || n > 24)
        return (seq_t)0;

    seq_t count = 0;
    for (idx_t i = 1; i <= 7; ++i)
        for (idx_t j = i + 1; j <= 8; ++j)
            for (idx_t k = j + 1; k <= 9; ++k)
                if (i + j + k == n)
                    ++count;

    return count;
}
