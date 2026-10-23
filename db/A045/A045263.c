/*
 * A045263  Numbers whose base-5 representation contains no 2's and exactly
 *          one 3.
 * https://oeis.org/A045263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base-5 representation contains no 2's and exactly
 *            one 3
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static unsigned char pow3_arr[64];
static bool pow3_init = false;
static void init_pow3(void)
{
    if (pow3_init) return;
    pow3_arr[0] = 1;
    for (int i = 1; i < 64; ++i)
        pow3_arr[i] = pow3_arr[i - 1] * 3;
    pow3_init = true;
}

/* number of sequences of length len when we still need a 3
   (need3==true) or not (need3==false) */
static uint64_t count_rem(uint64_t len, bool need3)
{
    if (len == 0)
        return need3 ? 0 : 1;
    if (need3)
        return len * (uint64_t)pow3_arr[len - 1];
    else
        return (uint64_t)pow3_arr[len];
}

seq_t a(idx_t n)
{
    init_pow3();

    /* find the length of the representation */
    uint64_t cumulative = 0;
    uint64_t len = 1;
    for (;; ++len)
    {
        uint64_t cnt;
        if (len == 1)
            cnt = 1;
        else
            cnt = (uint64_t)pow3_arr[len - 1] + 2 * (len - 1) * (uint64_t)pow3_arr[len - 2];
        if (n <= cumulative + cnt)
            break;
        cumulative += cnt;
    }

    if (len == 1)                /* only the number 3 */
        return (seq_t)3;

    uint64_t offset = n - cumulative - 1;  /* zero‑based offset within length len */
    seq_t result = 0;
    bool need3 = true;                /* we still need a 3 */

    /* Choose the most significant digit */
    static const unsigned msbs[3] = {1, 3, 4};
    for (int i = 0; i < 3 && len > 0; ++i)
    {
        unsigned d = msbs[i];
        uint64_t cnt;
        if (d == 3)
            cnt = (uint64_t)pow3_arr[len - 1];
        else
            cnt = (len - 1) * (uint64_t)pow3_arr[len - 2];

        if (cnt <= offset)
        {
            offset -= cnt;
            continue;
        }

        result = result * 5 + (seq_t)d;
        need3 = (d != 3);
        --len;
        break;
    }

    /* Remaining digits */
    while (len > 0)
    {
        static const unsigned digits[4] = {0, 1, 3, 4};
        for (int i = 0; i < 4 && len > 0; ++i)
        {
            unsigned d = digits[i];
            if (!need3 && d == 3)
                continue;

            bool next_need3 = need3 && (d != 3);
            uint64_t cnt = count_rem(len - 1, next_need3);

            if (cnt <= offset)
            {
                offset -= cnt;
                continue;
            }

            result = result * 5 + (seq_t)d;
            need3 = next_need3;
            --len;
            break;
        }
    }

    return result;
}
