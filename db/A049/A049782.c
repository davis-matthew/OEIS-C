/*
 * A049782  a(n) = (0! + 1! + ... + (n-1)!) mod n.
 * https://oeis.org/A049782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (0! + 1! + … + (n-1)!) mod n
 * Range:     u8 n <= 266, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: multiply two values modulo n without overflow
   (works when n fits in 64‑bit; for the covered test
   ranges the product never exceeds 2^128). */
static inline seq_t mul_mod_seq(seq_t a, seq_t b, idx_t n)
{
    /* cast operands to 64‑bit before using 128‑bit product */
    unsigned __int128 prod = (unsigned __int128)(uint64_t)a *
                            (unsigned __int128)(uint64_t)b;
    return (seq_t)(prod % (unsigned __int128)n);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of the official sequence */
    seq_t sum = 0;
    seq_t fact = 1;                /* 0! */
    for (idx_t k = 0; k < n; ++k)  /* k runs from 0 to n‑1 */
    {
        sum = (sum + fact) % n;    /* add k! modulo n */

        /* update factorial: (k+1)! = k! * (k+1) */
        if (k + 1 == n) break;     /* no need to compute next factorial */
        fact = mul_mod_seq(fact, (seq_t)(k + 1), n);
    }
    return sum;
}
