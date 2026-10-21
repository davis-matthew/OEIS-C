/*
 * A074676  Differences between consecutive three-digit distinct-digit primes.
 * https://oeis.org/A074676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = difference between consecutive 3‑digit primes that have
 *            all distinct digits. a(1) is the difference between the first
 *            two such primes; the sequence ends with a(96)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static int primes[100];          /* 3‑digit primes with distinct digits (max 97 of them) */
    static seq_t diffs[96];          /* the differences between consecutive primes, size 96 */
    static int count = 0;
    static bool initialized = false;

    if (!initialized)
    {
        /* Sieve of Eratosthenes up to 999 */
        bool isPrime[1000];
        for (int i = 2; i < 1000; ++i) isPrime[i] = true;
        for (int p = 2; p * p < 1000; ++p)
            if (isPrime[p])
                for (int q = p * p; q < 1000; q += p)
                    isPrime[q] = false;

        /* collect 3‑digit primes with all distinct digits */
        for (int p = 100; p < 1000; ++p)
        {
            if (!isPrime[p]) continue;
            int tmp = p, seen = 0, ok = 1;
            while (tmp > 0)
            {
                int dig = tmp % 10;
                if (seen & (1 << dig)) { ok = 0; break; }
                seen |= 1 << dig;
                tmp /= 10;
            }
            if (ok) primes[count++] = p;
        }

        /* compute differences */
        for (int i = 1; i < count; ++i)
            diffs[i - 1] = (seq_t)(primes[i] - primes[i - 1]);

        initialized = true;
    }

    if (n < 1 || n > (idx_t)count - 1) return 0;
    return diffs[(size_t)n - 1];
}
