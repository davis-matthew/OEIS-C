/*
 * A089182  Prime digit palindromes 2,...,23577532 continued by adding
 *          10^(n-k) and 10^(k-1) times prime(k).
 * https://oeis.org/A089182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k=1..⌊n/2⌋} prime(k)*(10^(n‑k)+10^(k‑1)) + (n mod
 *            2)*prime((n+1)/2)*10^⌊n/2⌋
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k) */
#include "../utils/pow10.h"       /* seq_t pow10(idx_t e) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* offset is 1, undefined for n<1 */

    idx_t half = n >> 1;              /* ⌊n/2⌋ */
    seq_t result = 0;

    for (idx_t k = 1; k <= half; ++k) {
        seq_t p = (seq_t)get_prime(k);

        seq_t t1 = pow10(n - k);      /* 10^(n‑k) */
        seq_t t2 = pow10(k - 1);      /* 10^(k‑1) */

        result += p * (t1 + t2);      /* add prime(k)*(t1+t2) */
    }

    if (n & 1) {                      /* odd n: add middle term */
        seq_t p = (seq_t)get_prime((n + 1) >> 1);
        result += p * pow10(half);    /* prime((n+1)/2) * 10^⌊n/2⌋ */
    }

    return result;
}
