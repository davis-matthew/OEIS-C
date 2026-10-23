/*
 * A147881  Number of consistent sets of 8 irreflexive binary order
 *          relationships over n objects.
 * https://oeis.org/A147881
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-4)*(n-3)*(n-2)*(n-1)*n*(n^11 + 2*n^10 - 43*n^9 -
 *            226*n^8 - 21*n^7 + 5110*n^6 + 36247*n^5 + 91954*n^4 - 612172*n^3
 *            - 5449152*n^2 - 2743020*n + 82544280)/40320
 * Range:     u8 none, u16 n <= 5, u32 n <= 8, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   gcd, unsigned_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/unsigned_gcd.h"

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 5) return 0;          /* sequence offset */

    seq_t nt = (seq_t)n;
    seq_t denom = (seq_t)40320;
    seq_t pref = 1;

    /* pref = (n-4)*(n-3)*(n-2)*(n-1)*n with cancellation against denom */
    for (int i = 0; i < 5; ++i) {
        seq_t term = nt - (seq_t)i;
        seq_t g = unsigned_gcd(term, denom);
        if (g > 1) {
            term /= g;
            denom /= g;
        }
        pref *= term;
    }

    /* further cancel any remaining common factor */
    seq_t g = unsigned_gcd(pref, denom);
    if (g > 1) {
        pref /= g;
        denom /= g;
    }

    /* Evaluate the 11‑degree polynomial with a signed intermediate.
       Use 128‑bit signed on compilers that support it; otherwise use 64‑bit signed.
       The intermediate value should fit for all n that produce a result within seq_t. */
#if defined(__SIZEOF_INT128__)
    __int128 poly = 1;
    poly = poly * ( __int128)nt + 2;
    poly = poly * ( __int128)nt - 43;
    poly = poly * ( __int128)nt - 226;
    poly = poly * ( __int128)nt - 21;
    poly = poly * ( __int128)nt + 5110;
    poly = poly * ( __int128)nt + 36247;
    poly = poly * ( __int128)nt + 91954;
    poly = poly * ( __int128)nt - 612172;
    poly = poly * ( __int128)nt - 5449152;
    poly = poly * ( __int128)nt - 2743020;
    poly = poly * ( __int128)nt + 82544280;

    __int128 prod = ( __int128)pref * poly;
    seq_t result = (seq_t)(prod / ( __int128)denom);
    return result;
#else
    long long poly = 1;
    poly = poly * (long long)nt + 2;
    poly = poly * (long long)nt - 43;
    poly = poly * (long long)nt - 226;
    poly = poly * (long long)nt - 21;
    poly = poly * (long long)nt + 5110;
    poly = poly * (long long)nt + 36247;
    poly = poly * (long long)nt + 91954;
    poly = poly * (long long)nt - 612172;
    poly = poly * (long long)nt - 5449152;
    poly = poly * (long long)nt - 2743020;
    poly = poly * (long long)nt + 82544280;

    long long prod = pref * poly;
    seq_t result = (seq_t)(prod / (long long)denom);
    return result;
#endif
}
