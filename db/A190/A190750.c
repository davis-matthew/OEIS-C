/*
 * A190750  n+[nr/w]+[ns/w]+[nt/w]+[nu/w]+[nv/w], where r=sinh(1), s=cosh(1),
 *          t=tanh(1), u=csch(1), v=sech(1), w=coth(1).
 * https://oeis.org/A190750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n* (sinh(1)/coth(1))⌋ + ⌊n* (cosh(1)/coth(1))⌋ + ⌊n*
 *            (tanh(1))^2⌋ + ⌊n* (csch(1)/coth(1))⌋ + ⌊n* (sech(1)/coth(1))⌋
 * Range:     u8 n <= 53, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* constants are evaluated only once   */
    static int init = 0;
    static long double c1, c2, c3, c4, c5; /* the five irrational factors */

    if (!init) {
        long double sinh1  = sinhl(1.0L);
        long double cosh1  = coshl(1.0L);
        long double tanh1  = sinh1 / cosh1;
        long double coth1  = cosh1 / sinh1;
        long double csch1  = 1.0L / sinh1;
        long double sech1  = 1.0L / cosh1;

        c1 = sinh1 / coth1;          /* sinh(1)/coth(1) */
        c2 = cosh1 / coth1;          /* cosh(1)/coth(1) */
        c3 = tanh1 * tanh1;          /* tanh(1)^2 */
        c4 = csch1 / coth1;          /* csch(1)/coth(1) */
        c5 = sech1 / coth1;          /* sech(1)/coth(1) */

        init = 1;
    }

    unsigned long long nu = (unsigned long long)n;
    seq_t      result = (seq_t)nu;

    /* add the integer parts of n * ci  */
    result += (seq_t)((unsigned long long)(nu * c1));
    result += (seq_t)((unsigned long long)(nu * c2));
    result += (seq_t)((unsigned long long)(nu * c3));
    result += (seq_t)((unsigned long long)(nu * c4));
    result += (seq_t)((unsigned long long)(nu * c5));

    return result;
}
