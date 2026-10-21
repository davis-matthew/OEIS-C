/*
 * A074669  Six-digit distinct-digit primes.
 * https://oeis.org/A074669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = the nth six‑digit prime whose decimal digits are all
 *            distinct, in increasing order. The sequence has 10 239 terms
 * Range:     u8 none, u16 none, u32 n <= 9702, u64 n <= 9702, u128 n <= 9702,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds a static table of the six‑digit distinct‑digit
   primes once, and returns table[n‑1] for any n ≥ 1 (0 is returned if
   n is larger than the table size). */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static table holds all 10 239 primes.  Allocate a little more to
       be safe (11000). */
    static bool initialized = false;
    static seq_t primes[11000];
    static size_t primes_cnt = 0;

    if (!initialized) {
        initialized = true;

        /* Sieving primes up to one million is enough to cover all
           six‑digit primes. */
        const int LIMIT = 1000000;
        bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
        for (int i = 0; i <= LIMIT; ++i) is_prime[i] = true;
        is_prime[0] = false;
        is_prime[1] = false;

        for (int i = 2; i * i <= LIMIT; ++i) {
            if (is_prime[i]) {
                for (int j = i * i; j <= LIMIT; j += i)
                    is_prime[j] = false;
            }
        }

        /* Collect primes between 100000 and LIMIT with all distinct digits. */
        size_t cnt = 0;
        for (int x = 100001; x <= LIMIT && cnt < 11000; ++x) {
            if (is_prime[x]) {
                /* Check distinct decimal digits. */
                uint8_t seen[10] = {0};
                int y = x;
                bool ok = true;
                while (y > 0) {
                    int d = y % 10;
                    if (seen[d]) { ok = false; break; }
                    seen[d] = 1;
                    y /= 10;
                }
                if (ok) primes[cnt++] = (seq_t)x;
            }
        }
        free(is_prime);
        primes_cnt = cnt;
    }

    if (n == 0 || (idx_t)n > (idx_t)primes_cnt)
        return (seq_t)0;   /* undefined for out‑of‑range indices */

    return primes[n - 1];
}
