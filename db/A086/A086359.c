/*
 * A086359  Fixed point if the decimal-digit-sum function is iterated starting
 *          from A000984(n) = C(2n,n).
 * https://oeis.org/A086359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = digital_root( C(2n, n) ) where C(2n, n) is the central
 *            binomial coefficient. The digital root is obtained by iterating
 *            the decimal‑digit sum until a single digit is reached. It is
 *            equivalent to 1 + ((C(2n, n) - 1) mod 9), with the convention
 *            that a result of 0 corresponds to the digit 9
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t etc. */

static const int inv9[9] = {0, 1, 5, 0, 7, 2, 0, 4, 8};

/* Return the digital root of C(2n, n).  */
seq_t a(idx_t n)
{
    if (n == 0)                       /* not used for the sequence but safe */
        return 0;

    int e = 0;        /* exponent of 3 in the binomial coefficient */
    int prod = 1;     /* product of the remaining coprime factors modulo 9 */

    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t a_num = n + i;   /* numerator term */
        idx_t b_den = i;       /* denominator term */

        /* Remove all factors of 3 from the numerator and count them. */
        while (a_num % 3 == 0)
        {
            a_num /= 3;
            ++e;
        }

        /* Remove all factors of 3 from the denominator and subtract them. */
        while (b_den % 3 == 0)
        {
            b_den /= 3;
            --e;
        }

        /* Now a_num and b_den are coprime with 3, hence invertible modulo 9. */
        int a_mod = (int)(a_num % 9);
        int b_mod = (int)(b_den % 9);

        prod = (prod * a_mod) % 9;

        /* multiply by the modular inverse of b_mod modulo 9 */
        prod = (prod * inv9[b_mod]) % 9;
    }

    int mod9;
    if (e >= 2)
        mod9 = 0;                     /* divisible by 9 */
    else
    {
        mod9 = prod;
        if (e == 1)
            mod9 = (mod9 * 3) % 9;    /* exactly one factor of 3 present */
    }

    seq_t result = (seq_t)mod9;
    if (result == 0)                  /* digital root of a multiple of 9 is 9 */
        result = 9;

    return result;
}
