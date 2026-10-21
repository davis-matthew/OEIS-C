/*
 * A079303  a(n) = ((p-1)!/(2*p^2)) * Sum_{j=1..p-1} 1/j where p is the n-th
 *          prime.
 * https://oeis.org/A079303
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((p-1)!/(2*p^2)) * Sum_{j=1..p-1} 1/j where p =
 *            A000040(n) is the n‑th prime. The computation uses the
 *            recurrence fac_i = (i-1)! , val_i = fac_i * H_{i-1} (initially 0
 *            for i=1) For i = 1 .. p-1 fac_next = fac_i * i // (i)! val_next
 *            = i * val_i + fac_i // i! * H_i After the loop, val_{p-1}...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* A000040(1)=2, A000040(2)=3 – the sequence is defined for n≥3. */
    if (n < 3)
        return (seq_t)0;

    uint32_t p = get_prime(n);                 /* the n‑th prime          */
    const unsigned __int128 denom128 = (unsigned __int128)2 * p * p;

    /* Branch according to the size of seq_t. */
    if (sizeof(seq_t) <= 8)                      /* 64‑bit or smaller */
    {
        unsigned __int128 fac = 1;
        unsigned __int128 val = 0;

        for (uint32_t i = 1; i < p; ++i)
        {
            unsigned __int128 fac_next = fac * i;          /* i!                     */
            unsigned __int128 val_next = (unsigned __int128)i * val + fac;
            fac  = fac_next;
            val  = val_next;
        }
        val /= denom128;                                /* exact division          */
        return (seq_t)val;
    }
    else                                            /* arbitrary‑precision   */
    {
        seq_t fac = (seq_t)1;
        seq_t val = (seq_t)0;

        for (uint32_t i = 1; i < p; ++i)
        {
            seq_t fac_next = fac * (seq_t)i;          /* i!                      */
            val = (seq_t)i * val + fac;               /* i! * H_i                */
            fac = fac_next;
        }
        seq_t denom = (seq_t)2 * (seq_t)p * (seq_t)p;
        return val / denom;
    }
}
