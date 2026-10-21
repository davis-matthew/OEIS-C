/*
 * A236186  Differences between terms of compacting Eratosthenes sieve for
 *          prime(5) = 11.
 * https://oeis.org/A236186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) gives the differences between consecutive integers that are
 *            not divisible by any of 2, 3, 5, or 7 (wheel of modulo 210). The
 *            sequence is periodic with period 48: a(1)=2, a(2)=4, a(3)=2,
 *            a(4)=4, a(5)=6, a(6)=2, …, a(48)=10 and a(n+48)=a(n)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Period of the sequence: 48 */
    static seq_t period[48];
    static int initialized = 0;

    if (!initialized)
    {
        /* Compute residues modulo 210 that are coprime to 210.
           Their number is φ(210)=48. */
        uint64_t residues[48];
        size_t rcnt = 0;
        for (idx_t i = 1; i <= 210; ++i)
        {
            /* skip multiples of 2,3,5,7 */
            if ((i % 2) && (i % 3) && (i % 5) && (i % 7))
                residues[rcnt++] = i;
        }

        /* Compute consecutive differences (with wrap‑around). */
        for (size_t i = 0; i < 48; ++i)
        {
            size_t next = (i + 1) % 48;
            if (i != 47)
                period[i] = (seq_t)(residues[next] - residues[i]);
            else
                period[i] = (seq_t)(210 + residues[0] - residues[i]);  /* wrap */
        }
        initialized = 1;
    }
    /* Use modulo 48 to cycle through the period. */
    return period[(size_t)(n % 48)];
}
