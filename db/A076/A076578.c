/*
 * A076578  Triangular numbers which are 4-almost primes.
 * https://oeis.org/A076578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n‑th triangular number T_m = m(m+1)/2 with Ω(T_m) =
 *            4 * (Ω = total number of prime factors counted with
 *            multiplicity). * The algorithm simply scans successive m, counts
 *            Ω(T_m) by summing Ω(m) and Ω(m+1) * (subtracting one for the
 *            single factor 2 after division), and returns the * nth...
 * Range:     u8 n <= 3, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cnt_omega_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cnt_omega_uint64.h"

/* Return the triangular number T_m as a seq_t without using division. */
static seq_t triangular(uint64_t m)
{
    if ((m & 1ULL) == 0)  /* m even: (m/2)*(m+1) */
        return (seq_t)(m / 2) * (seq_t)(m + 1);
    else                 /* m odd: m*((m+1)/2)    */
        return (seq_t)m * (seq_t)((m + 1) / 2);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0 */

    uint64_t count = 0;             /* terms found so far */
    uint64_t m = 1;                 /* current index for triangular number */

    while (1)
    {
        /* Ω(T_m) = Ω(m) + Ω(m+1) - 1 (exactly one factor 2 is removed) */
        uint64_t omega_m   = cnt_omega_uint64(m);
        uint64_t omega_mp  = cnt_omega_uint64(m + 1);
        if (omega_m + omega_mp - 1 == 4)
        {
            ++count;
            if (count == (uint64_t)n)
                return triangular(m);
        }
        ++m;
    }
}
