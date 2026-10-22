/*
 * A003133  Order of simple Chevalley group E_8(2).
 * https://oeis.org/A003133
 *
 * Offset:    75    (a(75) is the first term)
 * Keywords:  nonn, fini, full, cons
 * Formula:   digits of |E_8(2)| =
 *            2^120*(2^2-1)(2^8-1)(2^12-1)(2^14-1)(2^18-1)(2^20-1)(2^24-1)(2^30-1);
 *            a(n) = (n-74)-th decimal digit from the left
 * Range:     u8 n <= 149, u16 n <= 149, u32 n <= 149, u64 n <= 149,
 *            u128 n <= 149, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

static char a3133_digits[200];
static int a3133_len = 0;

static void a3133_init(void)
{
    uint32_t limb[20];
    int len = 1, i, k;
    int degs[8] = {2, 8, 12, 14, 18, 20, 24, 30};
    uint32_t mults[12];
    int nm = 0;
    limb[0] = 1;
    for (i = 0; i < 4; i++) mults[nm++] = (uint32_t)1 << 30;
    for (i = 0; i < 8; i++) mults[nm++] = ((uint32_t)1 << degs[i]) - 1u;
    for (k = 0; k < nm; k++) {
        uint64_t carry = 0;
        for (i = 0; i < len; i++) {
            uint64_t t = (uint64_t)limb[i] * mults[k] + carry;
            limb[i] = (uint32_t)(t % 1000000000u);
            carry = t / 1000000000u;
        }
        while (carry) {
            limb[len++] = (uint32_t)(carry % 1000000000u);
            carry /= 1000000000u;
        }
    }
    {
        int pos = 0;
        pos += sprintf(a3133_digits + pos, "%u", (unsigned)limb[len - 1]);
        for (i = len - 2; i >= 0; i--)
            pos += sprintf(a3133_digits + pos, "%09u", (unsigned)limb[i]);
        a3133_len = pos;
    }
}

seq_t a(idx_t n)
{
    idx_t pos;
    if (a3133_len == 0) a3133_init();
    if (n < 75) return (seq_t)0;
    pos = n - 75;
    if (pos >= (idx_t)a3133_len) return (seq_t)0;
    return (seq_t)(a3133_digits[pos] - '0');
}
