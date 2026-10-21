/*
 * A087371  Smallest k such that 10^k + 2n-1 is a prime >10, or 0 if no such k
 *          exists. Index of the smallest power of 10 which when increased by
 *          the n-th odd number yields a prime.
 * https://oeis.org/A087371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that 10^k + (2n-1) is a prime >10;
 *            a(n) = 0 if no such k exists. The function first rejects
 *            impossible cases using divisibility by 3 or 5, then searches k
 *            from 1 up to a modest limit (20) using big‑integer arithmetic.
 *            For n beyond this limit the sequence has been observ...
 * Range:     u8 n <= 54, u16 n <= 105, u32 n <= 99, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime_big, mul_small_wider
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t n); */
#include "../utils/mul_small_wider.h" /* seq_t mul_small_wider(seq_t a, idx_t mul); */

seq_t a(idx_t n)
{
    /* odd number 2n‑1 */
    seq_t odd = (seq_t)2 * n - (seq_t)1;

    /* Quick elimination: if odd ≡ 0 (mod 3) → 2n‑1 ≡ 0 (mod 3)
       ⇒ 10^k + odd ≡ 1 + 0 ≡ 1 (mod 3) never 0, but if odd ≡ 2 mod 3
       ⇒ 1 + 2 ≡ 0 (mod 3) → composite for all k. */
    if ((odd % (seq_t)3) == (seq_t)2)
        return (seq_t)0;

    /* If odd is a multiple of 5 then 10^k + odd ends in 5 → composite. */
    if ((odd % (seq_t)5) == (seq_t)0)
        return (seq_t)0;

    /* Search k = 1 … MAX_K */
    const idx_t MAX_K = 20;
    seq_t pow10 = (seq_t)10;            /* 10^1 */

    for (idx_t k = 1; k <= MAX_K; ++k)
    {
        seq_t cand = pow10 + odd;

        if (is_prime_big(cand))
            return (seq_t)k;

        /* Prepare next power of 10 */
        pow10 = mul_small_wider(pow10, (idx_t)10);
    }

    /* If no prime found within the bound, assume no such k exists */
    return (seq_t)0;
}
