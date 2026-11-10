/*
 * A065356  Final digits of A065355(n) (in reverse order) for sufficiently
 *          large n.
 * https://oeis.org/A065356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 9 - A025016(n) for n > 0, where A025016(n) is digit n
 *            (from the right) of the 10-adic sum of all factorials; a(0) = 6
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 932 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned A065356_digit(idx_t n)
{
    const idx_t keep_digits = n + 1;
    const uint32_t BASE = 10000;
    const uint32_t POW10[4] = {1, 10, 100, 1000};
    const size_t limbs = (size_t)((keep_digits + 3) / 4);

    uint32_t *fact = (uint32_t *)calloc(limbs, sizeof(uint32_t));
    uint32_t *sum  = (uint32_t *)calloc(limbs, sizeof(uint32_t));
    if (!fact || !sum) {
        free(fact); free(sum);
        return 0;
    }
    fact[0] = 1;
    sum[0]  = 1;

    uint64_t cumV5 = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t tmp = k;
        uint64_t inc = 0;
        while (tmp % 5 == 0) {
            ++inc;
            tmp /= 5;
        }
        cumV5 += inc;
        if (cumV5 > n) break;

        uint64_t carry = 0;
        for (size_t l = 0; l < limbs; ++l) {
            uint64_t prod = (uint64_t)fact[l] * k + carry;
            fact[l] = (uint32_t)(prod % BASE);
            carry   = prod / BASE;
        }
        carry = 0;
        for (size_t l = 0; l < limbs; ++l) {
            uint64_t sumv = (uint64_t)sum[l] + fact[l] + carry;
            sum[l] = (uint32_t)(sumv % BASE);
            carry  = sumv / BASE;
        }
    }

    size_t limb_idx = (size_t)(n / 4);
    size_t offset   = (size_t)(n % 4);
    unsigned digit  = (unsigned)((sum[limb_idx] / POW10[offset]) % 10);

    free(fact);
    free(sum);
    return digit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;
    return (seq_t)(9 - A065356_digit(n));
}
