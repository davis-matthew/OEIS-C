/*
 * A175376  Partial sums of A175375.
 * https://oeis.org/A175376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # integer triples (x,y,z) with x^4+y^4+z^4 <= n and -n <=
 *            x, y, z <= n. (Based on a(0)=1, a(1)=7, a(2)=19, ….)
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute v^4 values and their multiplicities (signs). */
    /* Since x^4 grows quickly, the maximum |x| needed is only floor(n^(1/4)). */
    /* For n up to 10,000 this is at most 10.  For general n the array size is generous. */
    static const int MAXV = 1024;     /* safe upper bound for most n */
    seq_t pow4[MAXV];
    unsigned char mult[MAXV];
    int maxv = 0;

    for (int v = 0; ; ++v) {
        seq_t p = (seq_t)v * v * v * v;
        if (p > (seq_t)n)
            break;
        pow4[maxv]    = p;
        mult[maxv]    = (v == 0) ? 1 : 2;
        ++maxv;
    }

    seq_t total = 0;

    for (int i = 0; i < maxv; ++i) {
        seq_t a_val = pow4[i];
        unsigned w_a = mult[i];
        for (int j = 0; j < maxv; ++j) {
            seq_t sum_ab = a_val + pow4[j];
            if (sum_ab > (seq_t)n)
                continue;
            unsigned w_ab = w_a * mult[j];
            seq_t rem = (seq_t)n - sum_ab;

            /* Count number of z such that z^4 <= rem, weighted by multiplicities. */
            seq_t cnt_z = 0;
            for (int k = 0; k < maxv; ++k) {
                if (pow4[k] <= rem)
                    cnt_z += mult[k];
                else
                    break;          /* values are increasing with k */
            }

            total += (seq_t)w_ab * cnt_z;
        }
    }

    return total;
}
