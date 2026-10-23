/*
 * A129843  a(n) = number of positive integers that are <= n and are coprime
 *          to n!! (n!! = A006882(n)).
 * https://oeis.org/A129843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Count of integers m ≤ n that are coprime to n!! (double
 *            factorial). For odd n this equals the number of powers of 2 ≤ n.
 *            For even n it equals the number of odd m ≤ n not divisible by
 *            any odd prime p ≤ n/2 (since 2 always divides n!!)
 * Range:     u8 n <= 4093, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static void _init_primes(idx_t up, idx_t *primes, size_t *cnt)
{
    bool used[5001] = {0};
    size_t pc = 0;
    for (idx_t i = 2; i <= up; ++i) {
        if (!used[i]) {
            primes[pc++] = i;
            if (i * i <= up) {
                for (idx_t j = i * i; j <= up; j += i)
                    used[j] = true;
            }
        }
    }
    *cnt = pc;
}

seq_t a(idx_t n)
{
    /* handle the offset safely */
    if (n == 0) return 0;

    /* odd n : powers of 2 */
    if (n & 1ULL) {
        idx_t m = 1;
        seq_t cnt = 0;
        while (m <= n) {
            ++cnt;
            m <<= 1;
        }
        return cnt;
    }

    /* even n : use inclusion‑exclusion over primes ≤ n/2 */
    static bool primed = false;
    static idx_t primes[700];
    static size_t prime_cnt = 0;

    if (!primed) {
        _init_primes(5000, primes, &prime_cnt);
        primed = true;
    }

    idx_t half = n / 2;
    seq_t cnt = 0;

    for (idx_t k = 1; k <= n; k += 2) {          /* only odd k, 2 already excluded */
        bool ok = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = primes[i];
            if (p > half)
                break;
            if (p == 2)
                continue;                         /* 2 irrelevant for odd k */
            if ((k % p) == 0) {
                ok = false;
                break;
            }
        }
        if (ok)
            ++cnt;
    }

    return cnt;
}
