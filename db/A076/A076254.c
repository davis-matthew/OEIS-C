/*
 * A076254  A list of six integers with property that for every 3 numbers
 *          a,b,c from the list axbxc is an integer, where axb=a*b/(a+b) and
 *          axbxc=a*b*c/(a*b+a*c+b*c). This is the so-called replus operation.
 * https://oeis.org/A076254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n)=n*a(1) for n=1,2,3,4; a(5)=6*a(1); a(6)=8*a(1)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 6, u128 n <= 6, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) is the least common multiple of all ab+ac+bc, where
   a, b, c taken from {1,2,3,4,6,8}. */

#include "../utils/gcd.h"

static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    return (a / gcd(a, b)) * b;
}

seq_t a(idx_t n)
{
    static seq_t base = 0;   /* a(1) */
    if (base == 0) {
        const seq_t vals[6] = {1, 2, 3, 4, 6, 8};
        seq_t lcm_val = 1;
        for (int i = 0; i < 6; ++i) {
            for (int j = i + 1; j < 6; ++j) {
                for (int k = j + 1; k < 6; ++k) {
                    seq_t ab = vals[i] * vals[j];
                    seq_t ac = vals[i] * vals[k];
                    seq_t bc = vals[j] * vals[k];
                    seq_t d = ab + ac + bc;
                    lcm_val = lcm_seq(lcm_val, d);
                }
            }
        }
        base = lcm_val; /* 248971642920 */
    }

    switch (n) {
        case 1: return base;          /* a(1)      */
        case 2: return base * 2;      /* a(2)      */
        case 3: return base * 3;      /* a(3)      */
        case 4: return base * 4;      /* a(4)      */
        case 5: return base * 6;      /* a(5) = 6*a(1) */
        case 6: return base * 8;      /* a(6) = 8*a(1) */
        default: return 0;            /* undefined beyond 6 */
    }
}
