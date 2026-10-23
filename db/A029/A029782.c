/*
 * A029782  Cubes such that digits of cube root of n appear in both n^(2/3)
 *          and n.
 * https://oeis.org/A029782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A029782: cubes such that digits of cube root of n appear in
    both n^(2/3) and n.
    The n-th term is the cube of the n-th integer m whose decimal
    digit set is contained in that of m^2 and m^3.
*/

#include <stdlib.h>
#include <stdint.h>

/* Return a bit mask of the distinct decimal digits of x.  Bits 0-9
   are used: bit i is set if digit i occurs in x.  For x==0 the
   mask is 1 (digit 0).  The mask is stored in an unsigned int,
   which is wide enough for 10 bits. */
static unsigned digit_mask(seq_t x)
{
    unsigned mask = 0;

    if (x == 0) {
        return 1;  /* only digit 0 */
    }

    while (x > 0) {
        seq_t d = x % 10;          /* last decimal digit */
        mask |= 1U << (unsigned)d;
        x /= 10;                   /* drop last digit */
    }
    return mask;
}

seq_t a(idx_t n)
{
    /*  Cache of computed sequence values.  */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t  count   = 0;   /* how many terms stored */
    static idx_t  next_m  = 0;   /* next m to test */

    /*  Grow the cache on first use.  */
    if (cache == NULL) {
        cap   = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
    }

    /*  Generate terms until we have at least n of them.  */
    while (count < n) {
        seq_t m        = (seq_t)next_m;
        seq_t m2       = m * m;
        seq_t m3       = m2 * m;          /* m³ */

        unsigned mask_m   = digit_mask(m);
        unsigned mask_m2  = digit_mask(m2);
        unsigned mask_m3  = digit_mask(m3);

        /*  The digit set of m must be contained in those of both m² and m³.  */
        if ( (mask_m & mask_m2) == mask_m && (mask_m & mask_m3) == mask_m ) {

            /*  Grow the cache if needed.  */
            if (count == cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cap);
            }
            cache[count++] = m3;          /* store the cube */
        }

        ++next_m;
    }

    return cache[n - 1];
}
