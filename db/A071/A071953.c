/*
 * A071953  Diagonal T(n,n-2) of triangle in A071951.
 * https://oeis.org/A071953
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (n-2)*(n-1)*n*(n+1)*(5*n^2 - 11*n + 3)/90
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 65, u64 n <= 2632,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute gcd of a (unsigned long long) and b (unsigned int) */
static unsigned int gcd_uint64(unsigned long long a, unsigned int b)
{
    while (b) {
        unsigned long long t = a % b;
        a = b;
        b = (unsigned int)t;
    }
    return (unsigned int)a;
}

/* Main entry for the sequence.
   Works for any width of seq_t (uint64_t, unsigned __int128,
   arbitrary-precision integer class, etc.). */
seq_t a(idx_t n)
{
    if (n < 3) return 0;            /* offset 3 – behaviour below is irrelevant */

    seq_t prod = 1;
    unsigned int rem = 90;

    /* factors: (n-2), (n-1), n, (n+1), (5*n^2 - 11*n + 3) */
    seq_t f1 = (seq_t)n - 2;
    seq_t f2 = (seq_t)n - 1;
    seq_t f3 = (seq_t)n;
    seq_t f4 = (seq_t)n + 1;

    /* compute 5*n^2 - 11*n + 3 taking care of possible overflow */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t f5 = 5 * n_sq - 11 * (seq_t)n + 3;

    /* array of factors to loop over */
    seq_t factors[5] = {f1, f2, f3, f4, f5};

    for (int i = 0; i < 5; ++i) {
        seq_t f = factors[i];
        /* reduce the current factor by any common divisor with rem */
        if (rem > 1) {
            unsigned long long rem_val = (unsigned long long)(f % rem);
            unsigned int g = gcd_uint64(rem_val, rem);
            if (g > 1) {
                f /= g;
                rem /= g;
            }
        }
        prod *= f;
    }

    /* any remaining divisor is applied now */
    if (rem > 1) {
        prod /= rem;
    }

    return prod;
}
