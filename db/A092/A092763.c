/*
 * A092763  a(n) = floor(3^n / n).
 * https://oeis.org/A092763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(3^n / n)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static const uint32_t BASE = 1000000000U;          /* 1e9, fits in 32 bits */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                        /* offset is 1, but safeguard */

    /* 3^n is stored in an array of digits in base 1e9 (little‑endian).   */
    /* 5000 digits are safe for n <= 10000 (≈0.477*n decimal digits).       */
    uint32_t digits[2000];
    size_t sz = 1;
    digits[0] = 1;

    /* multiply by 3, n times (O(n * digits)) */
    for (idx_t i = 0; i < n; ++i)
    {
        uint64_t carry = 0;
        for (size_t j = 0; j < sz; ++j)
        {
            uint64_t prod = (uint64_t)digits[j] * 3 + carry;
            digits[j]      = (uint32_t)(prod % BASE);
            carry          = prod / BASE;
        }
        if (carry)
        {
            digits[sz++] = (uint32_t)carry;
        }
    }

    /* divide the big integer by the small integer n (floor division)   */
    uint64_t rem = 0;
    for (size_t idx = sz; idx > 0; --idx)
    {
        size_t i = idx - 1;
        uint64_t cur = rem * BASE + digits[i];
        digits[i] = (uint32_t)(cur / n);
        rem       = cur % n;
    }

    /* remove possible leading zeros on the most significant side */
    while (sz > 1 && digits[sz - 1] == 0)
        --sz;

    /* convert the result back to seq_t using only seq_t arithmetic     */
    seq_t result = 0;
    for (size_t i = sz; i > 0; --i)
    {
        result = result * BASE + digits[i - 1];
    }

    return result;
}
