/*
 * A133639  Mobius transform of b(n) where b(8n + 1) = A080995(n).
 * https://oeis.org/A133639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) is multiplicative. a(2^e) = a(3^e) = -1 if e==1, 0 if e>1;
 *            a(p^e) = (-1)^e for every prime p>3. Equivalently, a(n)=0 if n
 *            has 4 or 9 as a divisor; otherwise a(n)=(-1)^{Ω(n)} where Ω
 *            counts prime factors >3
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Computes A133639: a(n) as described above. */
seq_t a(idx_t n)
{
    seq_t result = 1;

    /* factor 2 */
    if ((n & 1ULL) == 0) {
        unsigned e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            e++;
            if (e > 1) return 0;      /* 2^e with e>1 gives 0 */
        }
        if (e == 1) result = -result;
    }

    /* factor 3 */
    if (n % 3ULL == 0) {
        unsigned e = 0;
        while (n % 3ULL == 0) {
            n /= 3ULL;
            e++;
            if (e > 1) return 0;      /* 3^e with e>1 gives 0 */
        }
        if (e == 1) result = -result;
    }

    /* factor remaining odd primes >3 */
    for (idx_t p = 5; p <= n / p; p += 2) {
        if (n % p == 0) {
            unsigned e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            if (e & 1) result = -result;   /* multiply by (-1)^e */
        }
    }

    if (n > 1) {                /* remaining prime > sqrt(original n) */
        result = -result;       /* exponent 1 => odd */
    }

    return result;
}
