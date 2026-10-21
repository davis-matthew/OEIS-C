/*
 * A205546  Least positive integer k such that n divides k^k-j^j for some j in
 *          [1,k-1].
 * https://oeis.org/A205546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 2 such that there exists j with 1 ≤ j < k where
 *            k^k ≡ j^j (mod n). This is equivalent to the first repetition in
 *            the sequence m^m (mod n)
 * Range:     u8 n <= 9896, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t modpow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0; safe guard */

    /* Allocate an array indicating whether each residue modulo n has
       already occurred. Size n is sufficient because a repetition must
       occur by pigeonhole principle at most after n+1 terms. */
    bool *seen = (bool*)calloc((size_t)n, sizeof(bool));
    if (!seen) return 0;     /* allocation failure → 0 */

    seq_t answer = 0;
    uint64_t mod = (uint64_t)n;

    for (uint64_t k = 1; k <= mod + 1; ++k)
    {
        uint64_t r = modpow_u64(k, k, mod);      /* k^k mod n */
        size_t idx = (size_t)r;
        if (seen[idx])
        {
            answer = (seq_t)k;
            break;
        }
        seen[idx] = true;
    }

    free(seen);
    return answer;
}
