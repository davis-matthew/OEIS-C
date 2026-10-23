/*
 * A129808  Decimal expansion of constant equal to concatenated nonprimes.
 * https://oeis.org/A129808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn, base
 * Formula:   a(n) = the nth digit (1‑based) in the decimal concatenation of
 *            all non‑prime integers in increasing order
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided is_prime() utility */
#include "../utils/is_prime.h"

/* helper: number of decimal digits of a non‑negative integer */
static inline idx_t num_digits(idx_t x)
{
    if (x == 0) return 1;
    idx_t d = 0;
    while (x) { ++d; x /= 10; }
    return d;
}

/* helper: 10^e as seq_t (e is reasonably small) */
static inline seq_t pow10(idx_t e)
{
    seq_t p = 1;
    for (idx_t i = 0; i < e; ++i) p *= 10;
    return p;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1; safe fallback */

    idx_t seen = 0;                /* digits already counted */

    for (idx_t num = 1; ; ++num)  /* start from 1, the first non‑prime */
    {
        if (!is_prime(num))
        {
            idx_t len = num_digits(num);

            if (seen + len >= n)
            {
                /* position inside the current number (0‑based) */
                idx_t pos = n - seen - 1;

                /* extract that digit */
                idx_t power = len - pos - 1;            /* exponent for right shift */
                seq_t div = pow10(power);               /* 10^(power) */
                seq_t digit = ((seq_t)num / div) % 10;   /* remainder after division */

                return digit;
            }

            seen += len;
        }
    }
}
