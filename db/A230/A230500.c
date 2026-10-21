/*
 * A230500  Indices of "power odious-evil stability" of odd integers 2*n-1 > 1
 *          (see comment).
 * https://oeis.org/A230500
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9464, u16 n <= 9464, u32 n <= 9464, u64 n <= 9464,
 *            u128 n <= 9464, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A230500
 *  a(n) = maximal k such that the odd integer 2*n-1 and its first k powers
 *  all have the same parity of binary ones (evil or odious).
 *  Implementation uses an arbitrary‑precision integer represented by a
 *  static array of 64‑bit words to avoid overflow when n is large.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* parity of a 64‑bit word (number of 1‑bits mod 2) */
static inline unsigned parity_uint64(uint64_t x)
{
    return __builtin_parityll(x);
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;                 /* undefined for n < 2 */

    /* odd integer 2*n-1 */
    uint64_t m = (uint64_t)n * 2ULL - 1ULL;

    /* parity of the base */
    unsigned base_parity = parity_uint64(m);

    /* big integer stored in little‐endian 64‑bit words */
    static uint64_t big[512];   /* 512 words => 32768 bits, enough */
    size_t size = 1;
    big[0] = m;                 /* current power m^e */

    idx_t e = 1;                /* current exponent */

    for (;;)
    {
        /* compute parity of current power */
        unsigned curr_parity = 0;
        for (size_t i = 0; i < size; ++i)
            curr_parity ^= parity_uint64(big[i]);

        if (curr_parity != base_parity)
            return (seq_t)(e - 1);   /* maximal exponent found */

        /* multiply big by m to get next power */
        unsigned __int128 carry = 0;
        for (size_t i = 0; i < size; ++i)
        {
            unsigned __int128 prod = (unsigned __int128)big[i] * m + carry;
            big[i] = (uint64_t)prod;
            carry   = prod >> 64;
        }
        if (carry)
        {
            if (size < sizeof(big) / sizeof(big[0]))
                big[size++] = (uint64_t)carry;
        }

        ++e;
    }
}
