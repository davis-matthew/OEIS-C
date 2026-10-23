/*
 * A184836  a(n) = n + floor(n*t) + floor(n/t) + floor(n/t^2) + floor(n/t^3),
 *          where t is the pentanacci constant.
 * https://oeis.org/A184836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*t) + floor(n/t) + floor(n/t^2) + floor(n/t^3)
 * Range:     u8 n <= 66, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Scaling factor: 2^61 (~9.22e18) – larger than any practical n in the tests */
static const unsigned long long SCALE = (1ULL << 61);

/* Pentanacci constant t (high‑precision, long double) */
static const long double T  = 1.96594823664586304345201373900044710776479176847122L;
static const long double T2 = T * T;
static const long double T3 = T2 * T;

/* Scaled integer approximations (floor of exact value) */
static const unsigned long long T1S = (unsigned long long)(T  * SCALE - 1e-7L);
static const unsigned long long T2S = (unsigned long long)(T2 * SCALE - 1e-7L);
static const unsigned long long T3S = (unsigned long long)(T3 * SCALE - 1e-7L);

/*
 * Computes A184836(n) for n >= 1.
 * The implementation uses integer arithmetic only; all intermediate
 * products are performed in unsigned __int128 to avoid overflow.
 */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but we return 0 for n==0 for safety */
        return 0;

    unsigned __int128 un = n;

    /* floor(n * t)  = (n * T1S) / SCALE */
    unsigned long long floor_nt = (unsigned long long)((un * T1S) / SCALE);

    /* floor(n / t)  = (n * SCALE) / T1S */
    unsigned long long floor_n_over_t  = (unsigned long long)((un * SCALE) / T1S);

    /* floor(n / t^2) = (n * SCALE) / T2S */
    unsigned long long floor_n_over_t2 = (unsigned long long)((un * SCALE) / T2S);

    /* floor(n / t^3) = (n * SCALE) / T3S */
    unsigned long long floor_n_over_t3 = (unsigned long long)((un * SCALE) / T3S);

    seq_t result = (seq_t)n;
    result += floor_nt;
    result += floor_n_over_t;
    result += floor_n_over_t2;
    result += floor_n_over_t3;

    return result;
}
