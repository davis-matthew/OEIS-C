/*
 * A190739  a(n) = n + [n*s/r] + [n*t/r] + [n*u/r] + [n*v/r] + [n*w/r], where
 *          r=sin(x), s=cos(x), t=tan(x), u=csc(x), v=sec(x), w=cot(x),
 *          x=Pi/8.
 * https://oeis.org/A190739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·sin(x)/cos(x)⌋ + ⌊n·tan(x)/sin(x)⌋ +
 *            ⌊n·csc(x)/sin(x)⌋ + ⌊n·sec(x)/sin(x)⌋ + ⌊n·cot(x)/sin(x)⌋ where
 *            x = π/8. Simplified: a(n) = 5·n + ⌊n(√2+1)⌋ + ⌊n·√(4−2√2)⌋ +
 *            2·⌊2n√2⌋ + ⌊n(2+√2)^(3/2)⌋
 * Range:     u8 n <= 12, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* high‑precision constants */
    static const long double SQRT2 = 1.414213562373095048801688724209698078569671875376948073179L;
    static const long double W    = SQRT2 + 1.0L;                                     /* √2 + 1 */
    static const long double V    = sqrtl(4.0L - 2.0L * SQRT2);                        /* √(4‑2√2) */
    static const long double W3   = sqrtl(20.0L + 14.0L * SQRT2);                       /* (2+√2)^(3/2) = √(20+14√2) */

    /* scaling factor for integer arithmetic */
    static const unsigned long long SCALE = 1000000000000000000ULL;   /* 1e18 */

    /* integer approximations of constants multiplied by SCALE */
    static const unsigned long long P_W  = (unsigned long long)(W  * (long double)SCALE);
    static const unsigned long long P_V  = (unsigned long long)(V  * (long double)SCALE);
    static const unsigned long long P_S  = (unsigned long long)(SQRT2 * (long double)SCALE);
    static const unsigned long long P_W3 = (unsigned long long)(W3 * (long double)SCALE);

    seq_t sum = 5 * (seq_t)n;          /* 5n */

    /* floor(n·(√2+1)) */
    unsigned __int128 prod = (unsigned __int128)n * P_W;
    sum += (seq_t)(prod / SCALE);

    /* floor(n·√(4−2√2)) */
    prod = (unsigned __int128)n * P_V;
    sum += (seq_t)(prod / SCALE);

    /* floor(2n√2) */
    prod = (unsigned __int128)n * (2 * P_S);
    seq_t t = (seq_t)(prod / SCALE);
    sum += 2 * t;                       /* 2 · floor(2n√2) */

    /* floor(n·(2+√2)^(3/2)) */
    prod = (unsigned __int128)n * P_W3;
    sum += (seq_t)(prod / SCALE);

    return sum;
}
