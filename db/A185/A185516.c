/*
 * A185516  Number of (n+1)X5 0..2 arrays with every 2X2 subblock singular.
 * https://oeis.org/A185516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=61*a(n-1)-1038*a(n-2)-2374*a(n-3)+235515*a(n-4)
 *            -2269967*a(n-5)+4091988*a(n-6)+57111048*a(n-7)
 *            -371380096*a(n-8)+662678992*a(n-9)+937755776*a(n-10)
 *            -4856175360*a(n-11)+5080858624*a(n-12)+506806272*a(n-13)
 *            -2361655296*a(n-14); Initial values (offset 1): a(1)=2553,
 *            a(2)=115979, a(3)=2490449, a...
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_from_str.h"

static const int64_t coeff[14] = {
    61,          // a(n-1)
   -1038,       // a(n-2)
   -2374,       // a(n-3)
    235515,     // a(n-4)
   -2269967,    // a(n-5)
    4091988,    // a(n-6)
   57111048,    // a(n-7)
   -371380096,  // a(n-8)
    662678992,  // a(n-9)
    937755776,  // a(n-10)
   -4856175360, // a(n-11)
    5080858624, // a(n-12)
    506806272,  // a(n-13)
   -2361655296 // a(n-14)
};

static const char *init_str[14] = {
    "2553",
    "115979",
    "2490449",
    "80761059",
    "2183297449",
    "65652458491",
    "1894624012417",
    "56140705014995",
    "1649002255383641",
    "48754926946880491",
    "1438991178016735921",
    "42542454880958044611",
    "1257346657004360967433",
    "37177308864717054716635"
};

static seq_t init_val[14];
static bool init_done = false;

seq_t a(idx_t n)
{
    /* initialize initial values once */
    if (!init_done) {
        for (int i = 0; i < 14; ++i)
            init_val[i] = seq_from_str(init_str[i]);
        init_done = true;
    }

    if (n <= 14)
        return init_val[n - 1];

    /* prepare the last 14 terms: prev[0] = a(n-1), prev[1] = a(n-2), ... */
    seq_t prev[14];
    for (int i = 0; i < 14; ++i)
        prev[i] = init_val[13 - i];

    for (idx_t cur = 15; cur <= n; ++cur) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int i = 0; i < 14; ++i) {
            int64_t c = coeff[i];
            if (c >= 0) {
                sum_pos += prev[i] * (seq_t)c;
            } else {
                sum_neg += prev[i] * (seq_t)(-c);
            }
        }
        seq_t next = sum_pos - sum_neg;

        /* shift the circular buffer */
        for (int i = 13; i > 0; --i)
            prev[i] = prev[i - 1];
        prev[0] = next;
    }

    return prev[0];
}
