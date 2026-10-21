/*
 * A089992  Second prime divisor of numbers that are not powers of primes
 *          (A024619).
 * https://oeis.org/A089992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = second smallest prime divisor of the n-th non‑prime‑power
 *            integer (A089992 = A119288(A024619(n)))
 * Range:     u8 n <= 395, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* ---------- static data ---------------------------------- */
    static bool initialized = false;
    static uint32_t *spf = NULL;          /* smallest prime factor */
    static int *primes = NULL;           /* list of primes */
    static size_t primes_cap = 0, primes_cnt = 0;

    static uint32_t *nums = NULL;        /* non‑prime‑power numbers */
    static size_t nums_cap = 0, nums_cnt = 0;

    static seq_t *values = NULL;         /* second prime divisor of each number */
    static size_t values_cap = 0, values_cnt = 0;

    /* ---------- initialization -------------------------------- */
    if (!initialized) {
        const size_t MAX_LIMIT = 200000;            /* enough for n up to ~10000 */

        /* ---- smallest prime factor table ---- */
        spf = (uint32_t *)calloc(MAX_LIMIT + 1, sizeof(uint32_t));
        if (!spf) return (seq_t)0;                 /* allocation failure */

        primes_cap = 200;
        primes = (int *)malloc(primes_cap * sizeof(int));

        /* ---- linear sieve to compute spf and primes ---- */
        for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
            if (!spf[i]) {
                spf[i] = i;
                if (primes_cnt == primes_cap) {
                    primes_cap <<= 1;
                    primes = (int *)realloc(primes, primes_cap * sizeof(int));
                }
                primes[primes_cnt++] = i;
            }
            for (size_t j = 0; j < primes_cnt &&
                    (uint64_t)i * (uint64_t)primes[j] <= MAX_LIMIT &&
                    primes[j] <= spf[i]; ++j) {
                spf[i * primes[j]] = primes[j];
            }
        }

        /* ---- allocate storage for results ---- */
        nums_cap = MAX_LIMIT / 2;
        nums = (uint32_t *)malloc(nums_cap * sizeof(uint32_t));

        values_cap = MAX_LIMIT / 2;
        values = (seq_t *)malloc(values_cap * sizeof(seq_t));

        /* ---- fill lists of non‑prime‑power numbers and their values ---- */
        for (uint32_t m = 2; m <= MAX_LIMIT; ++m) {
            uint32_t p1 = spf[m];
            uint32_t temp = m / p1;
            /* strip all factors of the smallest prime */
            while (temp % p1 == 0) temp /= p1;
            if (temp == 1) continue;              /* m is a prime power */

            uint32_t p2 = spf[temp];               /* second distinct prime */

            if (nums_cnt == nums_cap) {
                nums_cap <<= 1;
                nums = (uint32_t *)realloc(nums, nums_cap * sizeof(uint32_t));
            }
            if (values_cnt == values_cap) {
                values_cap <<= 1;
                values = (seq_t *)realloc(values, values_cap * sizeof(seq_t));
            }
            nums[nums_cnt++] = m;
            values[values_cnt++] = (seq_t)p2;
        }

        initialized = true;
    }

    /* ---------- answer request -------------------------------- */
    if (n == 0 || (size_t)n > values_cnt)
        return (seq_t)0;   /* out‑of‑range request – not expected for tests */

    return values[n - 1];
}
