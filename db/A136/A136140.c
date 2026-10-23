/*
 * A136140  Length of the cycles in which finish the sequences a(k+1)=sopfr(n
 *          a(k)+1), with sopfr=A001414.
 * https://oeis.org/A136140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 29, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(k+1)=sopfr(n*a(k)+1).  a(n) is the length of the eventual cycle
   for the sequence generated with parameter n. */

#include <stdint.h>

/* Sum of prime factors with multiplicity (sopfr) for 64‑bit integers. */
static seq_t sopfr_uint64(uint64_t m)
{
    seq_t sum = 0;
    while ((m & 1ULL) == 0) {
        sum += 2;
        m >>= 1;
    }
    for (uint64_t p = 3; p <= m / p; p += 2) {
        while (m % p == 0) {
            sum += p;
            m /= p;
        }
    }
    if (m > 1) sum += m;
    return sum;
}

/* Compute the next element of the sequence for a given n. */
static seq_t next_val(idx_t n, seq_t cur)
{
    /*  n and cur are assumed to be small enough to fit in 64 bits
        when multiplied; otherwise the cast will truncate but this
        is acceptable for the intended range of n.  */
    uint64_t prod = (uint64_t)((seq_t)n * cur + (seq_t)1);
    return sopfr_uint64(prod);
}

seq_t a(idx_t n)
{
    /* Floyd’s cycle‑finding algorithm.  The start value is 1.  */
    seq_t tortoise = 1;                 /* a(1) */
    seq_t hare     = next_val(n, tortoise); /* a(2) */

    /* First phase: find a meeting point inside the cycle. */
    while (tortoise != hare) {
        tortoise = next_val(n, tortoise);
        hare = next_val(n, next_val(n, hare));
    }

    /* Second phase: find the length of the cycle. */
    seq_t lam = 1;
    seq_t current = next_val(n, tortoise);
    while (current != tortoise) {
        current = next_val(n, current);
        ++lam;
    }
    return lam;
}
