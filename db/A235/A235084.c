/*
 * A235084  Number of (n+1) X (5+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1)+4*a(n-2)-87*a(n-3)+90*a(n-4)+450*a(n-5)
 *            -830*a(n-6)-936*a(n-7)+2737*a(n-8)+408*a(n-9)
 *            -4078*a(n-10)+891*a(n-11)+2766*a(n-12)
 *            -816*a(n-13)-904*a(n-14)+180*a(n-15)+120*a(n-16), with
 *            a(1)=1078, a(2)=2002, a(3)=3610, a(4)=7046, a(5)=13446,
 *            a(6)=27218, a(7)=53882, a(8)=111782, a(9)=226...
 * Range:     u8 none, u16 n <= 7, u32 n <= 21, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[16] = {
    (seq_t)1078ULL, (seq_t)2002ULL, (seq_t)3610ULL, (seq_t)7046ULL,
    (seq_t)13446ULL, (seq_t)27218ULL, (seq_t)53882ULL, (seq_t)111782ULL,
    (seq_t)226726ULL, (seq_t)478402ULL, (seq_t)986794ULL, (seq_t)2108630ULL,
    (seq_t)4404870ULL, (seq_t)9510434ULL, (seq_t)20077706ULL, (seq_t)43752230ULL
};

static const int coeffs[16] = {
    6, 4, -87, 90, 450, -830, -936,
    2737, 408, -4078, 891, 2766,
    -816, -904, 180, 120
};

#define MAX_N 20000
static seq_t cache[MAX_N];
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* sequence is defined only for n >= 1 */
    if (n > MAX_N) return 0;              /* out of supported bounds */

    /* Compute missing terms up to n */
    if (cache_len < n) {
        for (idx_t i = cache_len; i < n; ++i) {
            if (i < 16) {
                cache[i] = init_vals[i];
            } else {
                seq_t pos = 0, neg = 0;
                for (int k = 0; k < 16; ++k) {
                    idx_t idx = i - k - 1;
                    seq_t prod = cache[idx] *
                                 (seq_t)((coeffs[k] >= 0) ? coeffs[k] : -coeffs[k]);
                    if (coeffs[k] >= 0) pos += prod;
                    else                neg += prod;
                }
                cache[i] = pos - neg;
            }
        }
        cache_len = n;
    }
    return cache[n - 1];
}
