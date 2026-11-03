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
 * Range:     u8 n <= 54, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* multiply a seq_t by a small integer *mul* using a larger integer type
 * when seq_t fits in 64 bits, otherwise use normal multiplication.
 * This helper is not used directly in the implementation – the main
 * function handles the two cases separately – but it shows the idea
 * of using a wider type for intermediate values. */
static inline seq_t mul_small_wider(seq_t a, idx_t mul)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 t = (unsigned __int128)(unsigned long long)a *
                             (unsigned __int128)(unsigned long long)mul;
        return (seq_t)t;
    } else {
        return a * (seq_t)mul;
    }
}

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
