/*
 * A101597  Number of consecutive composite numbers between balanced primes
 *          and their lower or upper prime neighbor.
 * https://oeis.org/A101597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A013632(A006562(n))-1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the number of consecutive composite numbers between a balanced prime
    and its nearer prime neighbour (i.e. the maximum of the two gaps minus one). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* external linkage – do not mark static on the function */
seq_t a(idx_t n)
{
    /* one‑time initialization */
    static bool initialized = false;
    static seq_t *band_vals = NULL;   /* values of a(n) for balanced primes */
    static idx_t  val_cnt   = 0;      /* how many values have been stored */

    if (!initialized)
    {
        /* upper limit – sufficient for the first ~40000 balanced primes
           (covers n up to about 10000 comfortably). */
        const idx_t LIMIT = 10000000ULL;          /* 10⁷ */

        /* ---------- sieve of Eratosthenes ---------- */
        bool *is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
        if (!is_prime) return 0;
        for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (idx_t i = 2; i * i <= LIMIT; ++i)
        {
            if (is_prime[i])
            {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    is_prime[j] = false;
            }
        }

        /* ---------- gather primes into a linear array ---------- */
        /* number of primes up to 10⁷ is about 664 579 – use 1.2 M for safety. */
        const idx_t PRIMES_CAP = 1200000;
        uint32_t *primes = (uint32_t*)malloc(PRIMES_CAP * sizeof(uint32_t));
        if (!primes) { free(is_prime); return 0; }
        idx_t primes_cnt = 0;

        for (idx_t i = 2; i <= LIMIT; ++i)
        {
            if (is_prime[i])
            {
                primes[primes_cnt++] = (uint32_t)i;
            }
        }

        /* ---------- compute balanced primes and their values ---------- */
        band_vals = (seq_t*)malloc(primes_cnt * sizeof(seq_t));
        for (idx_t i = 1; i + 1 < primes_cnt; ++i)
        {
            uint32_t p_prev = primes[i - 1];
            uint32_t p_mid  = primes[i];
            uint32_t p_next = primes[i + 1];

            /* balanced prime test: p_mid is the arithmetic mean */
            if ((uint64_t)p_prev + (uint64_t)p_next ==
                (uint64_t)2ULL * (uint64_t)p_mid)
            {
                seq_t gap1 = (seq_t)p_mid - (seq_t)p_prev - (seq_t)1U;
                seq_t gap2 = (seq_t)p_next - (seq_t)p_mid - (seq_t)1U;
                band_vals[val_cnt++] = (gap1 > gap2 ? gap1 : gap2);
            }
        }

        /* clean up */
        free(is_prime);
        free(primes);
        initialized = true;
    }

    /* validate index – 0 is not a legal position in OEIS offset=1 */
    if (n == 0) return 0;
    idx_t idx = n - 1;
    if (idx >= val_cnt) return 0;     /* request beyond pre‑computed range */
    return band_vals[idx];
}
