/*
 * A041815  Denominators of continued fraction convergents to sqrt(428).
 * https://oeis.org/A041815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 3701774*a(n-12) - a(n-24) (for n>23) Initial terms:
 *            a(0)=1, a(1)=1, a(2)=3, a(3)=13, a(4)=16, a(5)=93, a(6)=946,
 *            a(7)=4823, a(8)=5769, a(9)=27899, a(10)=61567, a(11)=89466,
 *            a(12)=3640207, a(13)=3729673, a(14)=11099553, a(15)=48127885,
 *            a(16)=59227438, a(17)=344265075, a(18)=3501878188, a(19...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


seq_t a(idx_t n)
{
    static const seq_t init[24] = {
        (seq_t)1ULL,
        (seq_t)1ULL,
        (seq_t)3ULL,
        (seq_t)13ULL,
        (seq_t)16ULL,
        (seq_t)93ULL,
        (seq_t)946ULL,
        (seq_t)4823ULL,
        (seq_t)5769ULL,
        (seq_t)27899ULL,
        (seq_t)61567ULL,
        (seq_t)89466ULL,
        (seq_t)3640207ULL,
        (seq_t)3729673ULL,
        (seq_t)11099553ULL,
        (seq_t)48127885ULL,
        (seq_t)59227438ULL,
        (seq_t)344265075ULL,
        (seq_t)3501878188ULL,
        (seq_t)17853656015ULL,
        (seq_t)21355534203ULL,
        (seq_t)103275792827ULL,
        (seq_t)227907119857ULL,
        (seq_t)331182912684ULL
    };
    
    if (n < 24)
        return init[(size_t)n];
    
    const seq_t coeff = (seq_t)3701774;
    seq_t buf[24];
    for (size_t i = 0; i < 24; ++i)
        buf[i] = init[i];
    
    for (idx_t i = 24; i <= n; ++i) {
        seq_t val = coeff * buf[(size_t)((i - 12) % 24)] - buf[(size_t)((i - 24) % 24)];
        buf[(size_t)(i % 24)] = val;
    }
    
    return buf[(size_t)(n % 24)];
}
