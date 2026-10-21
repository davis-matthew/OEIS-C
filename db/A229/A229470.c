/*
 * A229470  Positions of 2 in decimal expansion of 0.1231232331232332333...,
 *          whose definition is given below.
 * https://oeis.org/A229470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = C(p+3,3) + C(m,2) where p is the unique integer with
 *            p*(p+1)/2 < n <= (p+1)*(p+2)/2 and m = n - p*(p+1)/2 + 1
 * Range:     u8 n <= 52, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the nth term of OEIS sequence A229470
   (positions of 2 in 0.1231232331232332333...) */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) /* offset is 1, behavior for n<1 is undefined */
        return 0;

    /* Find the block index 'p' that contains position n */
    idx_t p = 0;
    while (1) {
        unsigned __int128 prod = (unsigned __int128)p * (p + 1);
        idx_t start = (idx_t)(prod / 2 + 1);  /* first index in block p */
        idx_t end   = start + p;              /* last index in block p */
        if (n <= end)
            break;
        ++p;
    }

    /* Compute offset within the block and corresponding 'm' */
    unsigned __int128 prod_start = (unsigned __int128)p * (p + 1);
    idx_t start = (idx_t)(prod_start / 2 + 1);
    idx_t offset = n - start;              /* 0 … p */
    idx_t m = offset + 2;                  /* 2 … p+2 */

    /* Compute binomial(p+3,3) = (p+1)(p+2)(p+3)/6 */
    unsigned __int128 c3 = (unsigned __int128)(p + 1)
                        * (p + 2)
                        * (p + 3);
    c3 /= 6;

    /* Compute binomial(m,2) = m*(m-1)/2 */
    unsigned __int128 c2 = (unsigned __int128)m * (m - 1);
    c2 /= 2;

    seq_t result = (seq_t)c3 + (seq_t)c2;
    return result;
}
