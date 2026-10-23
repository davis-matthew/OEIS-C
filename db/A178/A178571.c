/*
 * A178571  a(n) = number of 5-digit primes with digit sum n, where n runs
 *          through the non-multiples of 3 in the range [2..44].
 * https://oeis.org/A178571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = number of 5‑digit primes with digit sum equal to the n-th
 *            non‑multiple of 3 in [2..44]
 * Range:     u8 n <= 8, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static const unsigned int non_mults[29] = {
    2,4,5,7,8,10,11,13,14,16,17,19,20,22,23,25,26,28,29,
    31,32,34,35,37,38,40,41,43,44
};

seq_t a(idx_t n)
{
    static bool built = false;
    static seq_t cnt[45];

    if (!built) {
        for (int i = 0; i < 45; ++i) cnt[i] = 0;

        const idx_t LIMIT = 100000;   /* 5‑digit numbers up to 99999 */
        bool *is_prime = (bool*)malloc(LIMIT * sizeof(bool));
        if (!is_prime) return 0;      /* unlikely in tests */

        for (idx_t i = 0; i < LIMIT; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;

        for (idx_t p = 2; p * p < LIMIT; ++p) {
            if (is_prime[p]) {
                for (idx_t q = p * p; q < LIMIT; q += p) is_prime[q] = false;
            }
        }

        for (idx_t num = 10000; num < 100000; ++num) {
            if (is_prime[num]) {
                uint32_t tmp = (uint32_t)num;
                uint32_t s = 0;
                while (tmp) {
                    s += tmp % 10;
                    tmp /= 10;
                }
                if (s <= 44 && s % 3 != 0) {
                    cnt[s] += 1;
                }
            }
        }
        free(is_prime);
        built = true;
    }

    if (n == 0) return 0;          /* offset starts at 1 */
    if (n > 29) return 0;          /* sums beyond 44 or multiples of 3 */
    unsigned int sum = non_mults[n - 1];
    return cnt[sum];
}
