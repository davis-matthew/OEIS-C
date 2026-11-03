/*
 * A078786  Period of cycle of the inventory sequence (as in A063850) starting
 *          with n.
 * https://oeis.org/A078786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nice, nonn
 * Formula:   a(n) = period of the cycle of the inventory sequence starting
 *            from n (found by cycle detection)
 * Range:     u8 none, u16 none, u32 n <= 39, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t inventory(seq_t x)
{
    /* convert x to decimal digits, most significant first */
    int rev[128];
    int len_rev = 0;
    if (x == 0) {
        rev[len_rev++] = 0;
    } else {
        seq_t tmp = x;
        while (tmp) {
            rev[len_rev++] = (int)(tmp % 10);
            tmp /= 10;
        }
    }

    int digits[128];
    for (int i = 0; i < len_rev; ++i)
        digits[i] = rev[len_rev - 1 - i];

    /* count occurrences and remember first appearance order */
    int counts[10] = {0};
    bool seen[10] = {false};
    int order[10];
    int order_len = 0;
    for (int i = 0; i < len_rev; ++i) {
        int d = digits[i];
        if (!seen[d]) {
            seen[d] = true;
            order[order_len++] = d;
        }
        ++counts[d];
    }

    /* build the output number as a string first */
    char buf[256];
    int pos = 0;

    for (int k = 0; k < order_len; ++k) {
        int d = order[k];
        /* count is small (≤ number of digits), we can cast safely */
        unsigned long long cnt = (unsigned long long)counts[d];

        /* write count in decimal */
        char tmpbuf[32];
        int tmppos = 0;
        if (cnt == 0) {
            tmpbuf[tmppos++] = '0';
        } else {
            while (cnt) {
                tmpbuf[tmppos++] = '0' + (int)(cnt % 10);
                cnt /= 10;
            }
        }
        for (int i = tmppos - 1; i >= 0; --i)
            buf[pos++] = tmpbuf[i];

        /* write the digit itself */
        buf[pos++] = '0' + d;
    }

    /* convert string buffer to seq_t value */
    seq_t result = 0;
    for (int i = 0; i < pos; ++i)
        result = result * 10 + (buf[i] - '0');

    return result;
}

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;

    /* Floyd's cycle-finding algorithm */
    seq_t tortoise = inventory(cur);
    seq_t hare = inventory(inventory(cur));

    while (tortoise != hare) {
        tortoise = inventory(tortoise);
        hare = inventory(inventory(hare));
    }

    /* determine cycle length (lambda) */
    seq_t lambda = 1;
    hare = inventory(tortoise);
    while (hare != tortoise) {
        hare = inventory(hare);
        ++lambda;
    }

    return lambda;
}
