/*
 * A018610  Divisors of 722.
 * https://oeis.org/A018610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 722 in ascending order
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The only numbers that divide 722 are 2^e * 19^f with
 * e ∈ {0,1} and f ∈ {0,1,2}.  The 6 resulting values are
 * 1, 2, 19, 38, 361, 722.  This function constructs the
 * divisor list once and returns the n‑th element.  For n>6
 * it returns 0, as the sequence is finite. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The prime factors of 722: 2 and 19^2. */
    static const seq_t primes[2] = { (seq_t)2, (seq_t)19 };
    static seq_t divisors[6];
    static unsigned init = 0;

    if (!init) {
        size_t idx = 0;
        /* Enumerate all exponent pairs (e,f). */
        for (idx_t e = 0; e <= 1; ++e) {
            for (idx_t f = 0; f <= 2; ++f) {
                seq_t val = (seq_t)1;
                if (e) val *= primes[0];          /* multiply by 2 if e==1 */
                for (idx_t k = 0; k < f; ++k)
                    val *= primes[1];            /* multiply by 19 f times */
                divisors[idx++] = val;
            }
        }
        /* Sort the 6 values in ascending order. */
        for (size_t i = 0; i < 6; ++i) {
            for (size_t j = i + 1; j < 6; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        init = 1;
    }

    if (n >= 1 && n <= 6)
        return divisors[(size_t)(n - 1)];
    return (seq_t)0;
}
