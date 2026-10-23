/*
 * A159072  Count of numbers k in the range 1<=k<= n such that set of proper
 *          divisors of k is not a subset of the set of the proper divisors of
 *          n.
 * https://oeis.org/A159072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (# of k with 2 ≤ k ≤ n such that – if k is a prime
 *            power p^e, then p^(e‑1) divides n – otherwise (k composite, not
 *            a prime power) k divides n). The number 1 (k = 1) is always
 *            counted in a(n)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    if (n == 0) return 0;          /* safe guard for non‑positive n */

    for (idx_t k = 2; k <= n; ++k) {
        idx_t temp = k;
        idx_t prime = 0;
        int    exp   = 0;
        bool   composite = false;

        /* factor k to detect whether it is a prime power */
        for (idx_t d = 2; d * d <= temp; ++d) {
            if (temp % d == 0) {
                prime = d;
                do { temp /= d; exp++; } while (temp % d == 0);
                if (temp > 1) {          /* a second distinct prime factor */
                    composite = true;
                    break;
                }
                break;                    /* only one prime factor remains */
            }
        }

        if (!composite) {
            if (prime == 0) {            /* k itself is prime */
                continue;                /* primes always satisfy the condition */
            } else {                     /* k is a prime power p^exp */
                idx_t need = 1;
                if (exp > 1) {
                    for (int i = 1; i < exp; ++i)
                        need *= prime;      /* need = p^(exp‑1) */
                }
                if (n % need != 0) cnt++; /* fails the condition */
            }
        } else {                           /* k has at least two distinct primes */
            if (n % k != 0) cnt++;         /* fails the condition */
        }
    }

    /* k = 1 always contributes one to a(n) */
    cnt += 1;
    return (seq_t)cnt;
}
