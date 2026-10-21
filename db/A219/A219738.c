/*
 * A219738  Unmatched value maps: number of nX5 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal,
 *          vertical or antidiagonal neighbor in a random 0..1 nX5 array.
 * https://oeis.org/A219738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+21*a(n-2)+48*a(n-3)+14*a(n-4) *
 *            -69*a(n-5)-38*a(n-6)+68*a(n-7)+13*a(n-8) *
 *            -57*a(n-9)+37*a(n-10)-8*a(n-11)-2*a(n-12)+a(n-13) * initial
 *            values (a(1) .. a(14)): * a(1)=12, a(2)=60, a(3)=387, a(4)=2229,
 *            a(5)=13322, * a(6)=78661, a(7)=466288, a(8)=2760690,
 *            a(9)=16350693, * a(10)=9683072...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* size_t */
#include <stdint.h>  /* uint64_t */

#define MAX_N 20000              /* sufficient for all tested values (n≈10000) */
static seq_t cache[MAX_N + 1];
static unsigned char built[MAX_N + 1] = {0};

static const seq_t init_vals[15] = {
    0,                      /* dummy 0th entry */
    12,                     /* a(1) */
    60,                     /* a(2) */
    387,                    /* a(3) */
    2229,                   /* a(4) */
    13322,                  /* a(5) */
    78661,                  /* a(6) */
    466288,                 /* a(7) */
    2760690,                /* a(8) */
    16350693,               /* a(9) */
    96830726,               /* a(10) */
    573456240,              /* a(11) */
    3396136349,             /* a(12) */
    20112704280,            /* a(13) */
    119112043349            /* a(14) */
};

static void ensure_cache(idx_t n)
{
    if (n < 1) return;
    if (n > MAX_N) return;   /* beyond our static buffer – caller must not request such n */

    for (idx_t i = 1; i <= n; ++i) {
        if (built[i]) continue;      /* already computed */
        if (i <= 14) {
            cache[i] = init_vals[i];
        } else {
            seq_t val = cache[i-1];
            val += ((seq_t)21) * cache[i-2];
            val += ((seq_t)48) * cache[i-3];
            val += ((seq_t)14) * cache[i-4];
            val -= ((seq_t)69) * cache[i-5];
            val -= ((seq_t)38) * cache[i-6];
            val += ((seq_t)68) * cache[i-7];
            val += ((seq_t)13) * cache[i-8];
            val -= ((seq_t)57) * cache[i-9];
            val += ((seq_t)37) * cache[i-10];
            val -= ((seq_t)8)  * cache[i-11];
            val -= ((seq_t)2)  * cache[i-12];
            val += cache[i-13];
            cache[i] = val;
        }
        built[i] = 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
